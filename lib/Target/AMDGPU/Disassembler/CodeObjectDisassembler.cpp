//===-- CodeObjectDisassembler.cpp - Disassembler for HSA Code Object------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//
//
/// \file
///
/// This file contains definition for HSA Code Object Dissassembler
//
//===----------------------------------------------------------------------===//

#include "CodeObjectDisassembler.h"

#include "AMDGPU.h"
#include "Disassembler/CodeObject.h"
#include "Disassembler/AMDGPUDisassembler.h"
#include "InstPrinter/AMDGPUInstPrinter.h"
#include "MCTargetDesc/AMDGPUTargetStreamer.h"

#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCFixedLenDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Object/ELFObjectFile.h"
#include "llvm/Support/ELF.h"
#include "llvm/Support/TargetRegistry.h"

#define DEBUG_TYPE "amdgpu-disassembler"

using namespace llvm;

CodeObjectDisassembler::CodeObjectDisassembler(MCContext *C,
                                               StringRef TN,
                                               MCInstPrinter *IP,
                                               MCDisassembler *ID,
                                               MCTargetStreamer *TS)
  : Ctx(C), TripleName(TN), InstPrinter(IP), InstDisasm(ID),
    AsmStreamer(static_cast<AMDGPUTargetStreamer *>(TS)) {}

ErrorOr<CodeObjectDisassembler::SymbolsTy>
CodeObjectDisassembler::CollectSymbols(const HSACodeObject *CodeObject) {
  SymbolsTy Symbols;
  for (const auto &Symbol : CodeObject->symbols()) {
    auto AddressOr = Symbol.getAddress();
    if (!AddressOr)
      return object::object_error::parse_failed;

    auto NameOr = Symbol.getName();
    if (!NameOr)
      return object::object_error::parse_failed;
    if (NameOr->empty())
      continue;

    uint8_t SymbolType = CodeObject->getSymbol(Symbol.getRawDataRefImpl())->getType();
    Symbols.emplace_back(*AddressOr, *NameOr, SymbolType);
  }
  return Symbols;
}

std::error_code CodeObjectDisassembler::printNotes(const HSACodeObject *CodeObject) {
  for (const auto &Note: CodeObject->notes()) {
    if (!Note)
      return Note.getError();

    switch (Note->type) {
    case NT_AMDGPU_HSA_CODE_OBJECT_VERSION: {
      auto VersionOr = Note->as<amdgpu_hsa_code_object_version>();
      if (!VersionOr)
        return VersionOr.getError();

      AsmStreamer->EmitDirectiveHSACodeObjectVersion(
        (*VersionOr)->major_version,
        (*VersionOr)->minor_version);
      break;
    }

    case NT_AMDGPU_HSA_ISA: {
      auto IsaOr = Note->as<amdgpu_hsa_isa>();
      if (!IsaOr)
        return IsaOr.getError();

      AsmStreamer->EmitDirectiveHSACodeObjectISA(
        (*IsaOr)->major,
        (*IsaOr)->minor,
        (*IsaOr)->stepping,
        (*IsaOr)->getVendorName(),
        (*IsaOr)->getArchitectureName());
      break;
    }
    }
  }
  return std::error_code();
}

std::error_code CodeObjectDisassembler::printKernels(const HSACodeObject *CodeObject,
                                                     raw_ostream &ES) {
  auto SymbolsOr = CollectSymbols(CodeObject);
  if (!SymbolsOr)
    return SymbolsOr.getError();
  
  for (const auto &Sym : CodeObject->kernels()) {
    auto Kernel = KernelSym::asKernelSym(CodeObject->getSymbol(Sym.getRawDataRefImpl())).get();
    auto NameEr = Sym.getName();
    if (!NameEr)
      return object::object_error::parse_failed;

    auto KernelCodeTOr = Kernel->getAmdKernelCodeT(CodeObject);
    if (!KernelCodeTOr)
      return KernelCodeTOr.getError();

    auto CodeOr = CodeObject->getKernelCode(Kernel);
    if (!CodeOr)
      return CodeOr.getError();
    
    AsmStreamer->EmitAMDGPUSymbolType(*NameEr, Kernel->getType());

    AsmStreamer->EmitAMDKernelCodeT(*(*KernelCodeTOr));

    printKernelCode(
      *CodeOr,
      Kernel->getValue() + (*KernelCodeTOr)->kernel_code_entry_byte_offset,
      *SymbolsOr,
      ES);

  }
  return std::error_code();
}

void CodeObjectDisassembler::printKernelCode(ArrayRef<uint8_t> Bytes,
                                             uint64_t Address,
                                             SymbolsTy &Symbols,
                                             raw_ostream &ES) {
#ifdef NDEBUG
  const bool DebugFlag = false;
#endif

  const auto &Target = getTheGCNTarget();
  std::unique_ptr<MCRelocationInfo> RelInfo(
    Target.createMCRelocationInfo(TripleName, *Ctx));
  if (RelInfo) {
    std::unique_ptr<MCSymbolizer> Symbolizer(
      Target.createMCSymbolizer(
        TripleName, nullptr, nullptr, &Symbols, Ctx, std::move(RelInfo)));
    InstDisasm->setSymbolizer(std::move(Symbolizer));
  }

  AsmStreamer->getStreamer().EmitRawText("// Disassembly:");
  SmallString<40> InstStr, CommentStr, OutStr;
  uint64_t Index = 0;
  while (Index < Bytes.size()) {
    InstStr.clear();
    raw_svector_ostream IS(InstStr);
    CommentStr.clear();
    raw_svector_ostream CS(CommentStr);
    OutStr.clear();
    raw_svector_ostream OS(OutStr);

    MCInst Inst;
    uint64_t EatenBytesNum = 0;
    if (InstDisasm->getInstruction(Inst, EatenBytesNum,
                                   Bytes.slice(Index),
                                   Address,
                                   DebugFlag ? dbgs() : nulls(),
                                   CS)) {
      InstPrinter->printInst(&Inst, IS, "", InstDisasm->getSubtargetInfo());
    } else {
      IS << "\t// unrecognized instruction ";
      if (EatenBytesNum == 0)
        EatenBytesNum = 4;
    }
    assert(0 == EatenBytesNum % 4);

    OS << left_justify(IS.str(), 60) << format("// %012X:", Address);
    for (auto D : Bytes.slice(Index, EatenBytesNum / 4))
      OS << format(" %08X", D);

    if (!CS.str().empty())
      OS << " // " << CS.str();

    AsmStreamer->getStreamer().EmitRawText(OS.str());

    Address += EatenBytesNum;
    Index += EatenBytesNum / 4;
  }
}

std::error_code CodeObjectDisassembler::Disassemble(MemoryBufferRef Buffer,
                                                    raw_ostream &ES) {
  using namespace object;
  
  // Create ELF 64-bit low-endian object file
  std::error_code EC;
  HSACodeObject CodeObject(Buffer, EC);
  if (EC)
    return EC;

  if (EC = printNotes(&CodeObject))
    return EC;

  if (EC = printKernels(&CodeObject, ES))
    return EC;

  return std::error_code();
}
