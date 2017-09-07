//===- X86InstructionSelector.cpp ----------------------------*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
/// \file
/// This file implements the targeting of the InstructionSelector class for
/// X86.
/// \todo This should be generated by TableGen.
//===----------------------------------------------------------------------===//

#include "X86InstrBuilder.h"
#include "X86InstrInfo.h"
#include "X86RegisterBankInfo.h"
#include "X86RegisterInfo.h"
#include "X86Subtarget.h"
#include "X86TargetMachine.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelector.h"
#include "llvm/CodeGen/GlobalISel/Utils.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "X86-isel"

#include "llvm/CodeGen/GlobalISel/InstructionSelectorImpl.h"
<<<<<<< HEAD

using namespace llvm;
=======
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0

using namespace llvm;

namespace {

#define GET_GLOBALISEL_PREDICATE_BITSET
#include "X86GenGlobalISel.inc"
#undef GET_GLOBALISEL_PREDICATE_BITSET

class X86InstructionSelector : public InstructionSelector {
public:
  X86InstructionSelector(const X86TargetMachine &TM, const X86Subtarget &STI,
                         const X86RegisterBankInfo &RBI);

  bool select(MachineInstr &I) const override;

private:
  /// tblgen-erated 'select' implementation, used as the initial selector for
  /// the patterns that don't require complex C++.
  bool selectImpl(MachineInstr &I) const;

  // TODO: remove after supported by Tablegen-erated instruction selection.
  unsigned getLoadStoreOp(LLT &Ty, const RegisterBank &RB, unsigned Opc,
                          uint64_t Alignment) const;

  bool selectLoadStoreOp(MachineInstr &I, MachineRegisterInfo &MRI,
                         MachineFunction &MF) const;
  bool selectFrameIndexOrGep(MachineInstr &I, MachineRegisterInfo &MRI,
                             MachineFunction &MF) const;
  bool selectGlobalValue(MachineInstr &I, MachineRegisterInfo &MRI,
                         MachineFunction &MF) const;
  bool selectConstant(MachineInstr &I, MachineRegisterInfo &MRI,
                      MachineFunction &MF) const;
  bool selectTrunc(MachineInstr &I, MachineRegisterInfo &MRI,
                   MachineFunction &MF) const;
  bool selectZext(MachineInstr &I, MachineRegisterInfo &MRI,
                  MachineFunction &MF) const;
  bool selectCmp(MachineInstr &I, MachineRegisterInfo &MRI,
                 MachineFunction &MF) const;
  bool selectUadde(MachineInstr &I, MachineRegisterInfo &MRI,
                   MachineFunction &MF) const;
  bool selectCopy(MachineInstr &I, MachineRegisterInfo &MRI) const;
  bool selectUnmergeValues(MachineInstr &I, MachineRegisterInfo &MRI,
                           MachineFunction &MF) const;
  bool selectMergeValues(MachineInstr &I, MachineRegisterInfo &MRI,
                         MachineFunction &MF) const;
  bool selectInsert(MachineInstr &I, MachineRegisterInfo &MRI,
                    MachineFunction &MF) const;
  bool selectExtract(MachineInstr &I, MachineRegisterInfo &MRI,
                     MachineFunction &MF) const;
<<<<<<< HEAD
=======
  bool selectCondBranch(MachineInstr &I, MachineRegisterInfo &MRI,
                        MachineFunction &MF) const;
  bool selectImplicitDefOrPHI(MachineInstr &I, MachineRegisterInfo &MRI) const;
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0

  // emit insert subreg instruction and insert it before MachineInstr &I
  bool emitInsertSubreg(unsigned DstReg, unsigned SrcReg, MachineInstr &I,
                        MachineRegisterInfo &MRI, MachineFunction &MF) const;
  // emit extract subreg instruction and insert it before MachineInstr &I
  bool emitExtractSubreg(unsigned DstReg, unsigned SrcReg, MachineInstr &I,
                         MachineRegisterInfo &MRI, MachineFunction &MF) const;

  const TargetRegisterClass *getRegClass(LLT Ty, const RegisterBank &RB) const;
  const TargetRegisterClass *getRegClass(LLT Ty, unsigned Reg,
                                         MachineRegisterInfo &MRI) const;

  const X86TargetMachine &TM;
  const X86Subtarget &STI;
  const X86InstrInfo &TII;
  const X86RegisterInfo &TRI;
  const X86RegisterBankInfo &RBI;

#define GET_GLOBALISEL_PREDICATES_DECL
#include "X86GenGlobalISel.inc"
#undef GET_GLOBALISEL_PREDICATES_DECL

#define GET_GLOBALISEL_TEMPORARIES_DECL
#include "X86GenGlobalISel.inc"
#undef GET_GLOBALISEL_TEMPORARIES_DECL
};

} // end anonymous namespace

#define GET_GLOBALISEL_IMPL
#include "X86GenGlobalISel.inc"
#undef GET_GLOBALISEL_IMPL

X86InstructionSelector::X86InstructionSelector(const X86TargetMachine &TM,
                                               const X86Subtarget &STI,
                                               const X86RegisterBankInfo &RBI)
    : InstructionSelector(), TM(TM), STI(STI), TII(*STI.getInstrInfo()),
      TRI(*STI.getRegisterInfo()), RBI(RBI),
#define GET_GLOBALISEL_PREDICATES_INIT
#include "X86GenGlobalISel.inc"
#undef GET_GLOBALISEL_PREDICATES_INIT
#define GET_GLOBALISEL_TEMPORARIES_INIT
#include "X86GenGlobalISel.inc"
#undef GET_GLOBALISEL_TEMPORARIES_INIT
{
}

// FIXME: This should be target-independent, inferred from the types declared
// for each class in the bank.
const TargetRegisterClass *
X86InstructionSelector::getRegClass(LLT Ty, const RegisterBank &RB) const {
  if (RB.getID() == X86::GPRRegBankID) {
    if (Ty.getSizeInBits() <= 8)
      return &X86::GR8RegClass;
    if (Ty.getSizeInBits() == 16)
      return &X86::GR16RegClass;
    if (Ty.getSizeInBits() == 32)
      return &X86::GR32RegClass;
    if (Ty.getSizeInBits() == 64)
      return &X86::GR64RegClass;
  }
  if (RB.getID() == X86::VECRRegBankID) {
    if (Ty.getSizeInBits() == 32)
      return STI.hasAVX512() ? &X86::FR32XRegClass : &X86::FR32RegClass;
    if (Ty.getSizeInBits() == 64)
      return STI.hasAVX512() ? &X86::FR64XRegClass : &X86::FR64RegClass;
    if (Ty.getSizeInBits() == 128)
      return STI.hasAVX512() ? &X86::VR128XRegClass : &X86::VR128RegClass;
    if (Ty.getSizeInBits() == 256)
      return STI.hasAVX512() ? &X86::VR256XRegClass : &X86::VR256RegClass;
    if (Ty.getSizeInBits() == 512)
      return &X86::VR512RegClass;
  }

  llvm_unreachable("Unknown RegBank!");
}

const TargetRegisterClass *
X86InstructionSelector::getRegClass(LLT Ty, unsigned Reg,
                                    MachineRegisterInfo &MRI) const {
  const RegisterBank &RegBank = *RBI.getRegBank(Reg, MRI, TRI);
  return getRegClass(Ty, RegBank);
}

static unsigned getSubRegIndex(const TargetRegisterClass *RC) {
  unsigned SubIdx = X86::NoSubRegister;
  if (RC == &X86::GR32RegClass) {
    SubIdx = X86::sub_32bit;
  } else if (RC == &X86::GR16RegClass) {
    SubIdx = X86::sub_16bit;
  } else if (RC == &X86::GR8RegClass) {
    SubIdx = X86::sub_8bit;
  }

  return SubIdx;
}

static const TargetRegisterClass *getRegClassFromGRPhysReg(unsigned Reg) {
  assert(TargetRegisterInfo::isPhysicalRegister(Reg));
  if (X86::GR64RegClass.contains(Reg))
    return &X86::GR64RegClass;
  if (X86::GR32RegClass.contains(Reg))
    return &X86::GR32RegClass;
  if (X86::GR16RegClass.contains(Reg))
    return &X86::GR16RegClass;
  if (X86::GR8RegClass.contains(Reg))
    return &X86::GR8RegClass;

  llvm_unreachable("Unknown RegClass for PhysReg!");
}

// Set X86 Opcode and constrain DestReg.
bool X86InstructionSelector::selectCopy(MachineInstr &I,
                                        MachineRegisterInfo &MRI) const {

  unsigned DstReg = I.getOperand(0).getReg();
  const unsigned DstSize = RBI.getSizeInBits(DstReg, MRI, TRI);
  const RegisterBank &DstRegBank = *RBI.getRegBank(DstReg, MRI, TRI);

  unsigned SrcReg = I.getOperand(1).getReg();
  const unsigned SrcSize = RBI.getSizeInBits(SrcReg, MRI, TRI);
  const RegisterBank &SrcRegBank = *RBI.getRegBank(SrcReg, MRI, TRI);

  if (TargetRegisterInfo::isPhysicalRegister(DstReg)) {
    assert(I.isCopy() && "Generic operators do not allow physical registers");

    if (DstSize > SrcSize && SrcRegBank.getID() == X86::GPRRegBankID &&
        DstRegBank.getID() == X86::GPRRegBankID) {

      const TargetRegisterClass *SrcRC =
          getRegClass(MRI.getType(SrcReg), SrcRegBank);
      const TargetRegisterClass *DstRC = getRegClassFromGRPhysReg(DstReg);

      if (SrcRC != DstRC) {
        // This case can be generated by ABI lowering, performe anyext
        unsigned ExtSrc = MRI.createVirtualRegister(DstRC);
        BuildMI(*I.getParent(), I, I.getDebugLoc(),
                TII.get(TargetOpcode::SUBREG_TO_REG))
            .addDef(ExtSrc)
            .addImm(0)
            .addReg(SrcReg)
            .addImm(getSubRegIndex(SrcRC));

        I.getOperand(1).setReg(ExtSrc);
      }
    }

    return true;
  }

  assert((!TargetRegisterInfo::isPhysicalRegister(SrcReg) || I.isCopy()) &&
         "No phys reg on generic operators");
  assert((DstSize == SrcSize ||
          // Copies are a mean to setup initial types, the number of
          // bits may not exactly match.
          (TargetRegisterInfo::isPhysicalRegister(SrcReg) &&
           DstSize <= RBI.getSizeInBits(SrcReg, MRI, TRI))) &&
         "Copy with different width?!");

  const TargetRegisterClass *DstRC =
      getRegClass(MRI.getType(DstReg), DstRegBank);

  if (SrcRegBank.getID() == X86::GPRRegBankID &&
      DstRegBank.getID() == X86::GPRRegBankID && SrcSize > DstSize &&
      TargetRegisterInfo::isPhysicalRegister(SrcReg)) {
    // Change the physical register to performe truncate.

    const TargetRegisterClass *SrcRC = getRegClassFromGRPhysReg(SrcReg);

    if (DstRC != SrcRC) {
      I.getOperand(1).setSubReg(getSubRegIndex(DstRC));
      I.getOperand(1).substPhysReg(SrcReg, TRI);
    }
  }

  // No need to constrain SrcReg. It will get constrained when
  // we hit another of its use or its defs.
  // Copies do not have constraints.
  const TargetRegisterClass *OldRC = MRI.getRegClassOrNull(DstReg);
  if (!OldRC || !DstRC->hasSubClassEq(OldRC)) {
    if (!RBI.constrainGenericRegister(DstReg, *DstRC, MRI)) {
      DEBUG(dbgs() << "Failed to constrain " << TII.getName(I.getOpcode())
                   << " operand\n");
      return false;
    }
  }
  I.setDesc(TII.get(X86::COPY));
  return true;
}

bool X86InstructionSelector::select(MachineInstr &I) const {
  assert(I.getParent() && "Instruction should be in a basic block!");
  assert(I.getParent()->getParent() && "Instruction should be in a function!");

  MachineBasicBlock &MBB = *I.getParent();
  MachineFunction &MF = *MBB.getParent();
  MachineRegisterInfo &MRI = MF.getRegInfo();

  unsigned Opcode = I.getOpcode();
  if (!isPreISelGenericOpcode(Opcode)) {
    // Certain non-generic instructions also need some special handling.

    if (Opcode == TargetOpcode::LOAD_STACK_GUARD)
      return false;

    if (I.isCopy())
      return selectCopy(I, MRI);

    return true;
  }

  assert(I.getNumOperands() == I.getNumExplicitOperands() &&
         "Generic instruction has unexpected implicit operands\n");

  if (selectImpl(I))
    return true;

  DEBUG(dbgs() << " C++ instruction selection: "; I.print(dbgs()));

  // TODO: This should be implemented by tblgen.
  if (selectLoadStoreOp(I, MRI, MF))
    return true;
  if (selectFrameIndexOrGep(I, MRI, MF))
    return true;
  if (selectGlobalValue(I, MRI, MF))
    return true;
  if (selectConstant(I, MRI, MF))
    return true;
  if (selectTrunc(I, MRI, MF))
    return true;
  if (selectZext(I, MRI, MF))
    return true;
  if (selectCmp(I, MRI, MF))
    return true;
  if (selectUadde(I, MRI, MF))
    return true;
  if (selectUnmergeValues(I, MRI, MF))
    return true;
  if (selectMergeValues(I, MRI, MF))
    return true;
  if (selectExtract(I, MRI, MF))
    return true;
  if (selectInsert(I, MRI, MF))
    return true;
<<<<<<< HEAD
=======
  if (selectCondBranch(I, MRI, MF))
    return true;
  if (selectImplicitDefOrPHI(I, MRI))
    return true;
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0

  return false;
}

unsigned X86InstructionSelector::getLoadStoreOp(LLT &Ty, const RegisterBank &RB,
                                                unsigned Opc,
                                                uint64_t Alignment) const {
  bool Isload = (Opc == TargetOpcode::G_LOAD);
  bool HasAVX = STI.hasAVX();
  bool HasAVX512 = STI.hasAVX512();
  bool HasVLX = STI.hasVLX();

  if (Ty == LLT::scalar(8)) {
    if (X86::GPRRegBankID == RB.getID())
      return Isload ? X86::MOV8rm : X86::MOV8mr;
  } else if (Ty == LLT::scalar(16)) {
    if (X86::GPRRegBankID == RB.getID())
      return Isload ? X86::MOV16rm : X86::MOV16mr;
  } else if (Ty == LLT::scalar(32) || Ty == LLT::pointer(0, 32)) {
    if (X86::GPRRegBankID == RB.getID())
      return Isload ? X86::MOV32rm : X86::MOV32mr;
    if (X86::VECRRegBankID == RB.getID())
      return Isload ? (HasAVX512 ? X86::VMOVSSZrm
                                 : HasAVX ? X86::VMOVSSrm : X86::MOVSSrm)
                    : (HasAVX512 ? X86::VMOVSSZmr
                                 : HasAVX ? X86::VMOVSSmr : X86::MOVSSmr);
  } else if (Ty == LLT::scalar(64) || Ty == LLT::pointer(0, 64)) {
    if (X86::GPRRegBankID == RB.getID())
      return Isload ? X86::MOV64rm : X86::MOV64mr;
    if (X86::VECRRegBankID == RB.getID())
      return Isload ? (HasAVX512 ? X86::VMOVSDZrm
                                 : HasAVX ? X86::VMOVSDrm : X86::MOVSDrm)
                    : (HasAVX512 ? X86::VMOVSDZmr
                                 : HasAVX ? X86::VMOVSDmr : X86::MOVSDmr);
  } else if (Ty.isVector() && Ty.getSizeInBits() == 128) {
    if (Alignment >= 16)
      return Isload ? (HasVLX ? X86::VMOVAPSZ128rm
                              : HasAVX512
                                    ? X86::VMOVAPSZ128rm_NOVLX
                                    : HasAVX ? X86::VMOVAPSrm : X86::MOVAPSrm)
                    : (HasVLX ? X86::VMOVAPSZ128mr
                              : HasAVX512
                                    ? X86::VMOVAPSZ128mr_NOVLX
                                    : HasAVX ? X86::VMOVAPSmr : X86::MOVAPSmr);
    else
      return Isload ? (HasVLX ? X86::VMOVUPSZ128rm
                              : HasAVX512
                                    ? X86::VMOVUPSZ128rm_NOVLX
                                    : HasAVX ? X86::VMOVUPSrm : X86::MOVUPSrm)
                    : (HasVLX ? X86::VMOVUPSZ128mr
                              : HasAVX512
                                    ? X86::VMOVUPSZ128mr_NOVLX
                                    : HasAVX ? X86::VMOVUPSmr : X86::MOVUPSmr);
  } else if (Ty.isVector() && Ty.getSizeInBits() == 256) {
    if (Alignment >= 32)
      return Isload ? (HasVLX ? X86::VMOVAPSZ256rm
                              : HasAVX512 ? X86::VMOVAPSZ256rm_NOVLX
                                          : X86::VMOVAPSYrm)
                    : (HasVLX ? X86::VMOVAPSZ256mr
                              : HasAVX512 ? X86::VMOVAPSZ256mr_NOVLX
                                          : X86::VMOVAPSYmr);
    else
      return Isload ? (HasVLX ? X86::VMOVUPSZ256rm
                              : HasAVX512 ? X86::VMOVUPSZ256rm_NOVLX
                                          : X86::VMOVUPSYrm)
                    : (HasVLX ? X86::VMOVUPSZ256mr
                              : HasAVX512 ? X86::VMOVUPSZ256mr_NOVLX
                                          : X86::VMOVUPSYmr);
  } else if (Ty.isVector() && Ty.getSizeInBits() == 512) {
    if (Alignment >= 64)
      return Isload ? X86::VMOVAPSZrm : X86::VMOVAPSZmr;
    else
      return Isload ? X86::VMOVUPSZrm : X86::VMOVUPSZmr;
  }
  return Opc;
}

// Fill in an address from the given instruction.
static void X86SelectAddress(const MachineInstr &I,
                             const MachineRegisterInfo &MRI,
                             X86AddressMode &AM) {

  assert(I.getOperand(0).isReg() && "unsupported opperand.");
  assert(MRI.getType(I.getOperand(0).getReg()).isPointer() &&
         "unsupported type.");

  if (I.getOpcode() == TargetOpcode::G_GEP) {
    if (auto COff = getConstantVRegVal(I.getOperand(2).getReg(), MRI)) {
      int64_t Imm = *COff;
      if (isInt<32>(Imm)) { // Check for displacement overflow.
        AM.Disp = static_cast<int32_t>(Imm);
        AM.Base.Reg = I.getOperand(1).getReg();
        return;
      }
    }
  } else if (I.getOpcode() == TargetOpcode::G_FRAME_INDEX) {
    AM.Base.FrameIndex = I.getOperand(1).getIndex();
    AM.BaseType = X86AddressMode::FrameIndexBase;
    return;
  }

  // Default behavior.
  AM.Base.Reg = I.getOperand(0).getReg();
  return;
}

bool X86InstructionSelector::selectLoadStoreOp(MachineInstr &I,
                                               MachineRegisterInfo &MRI,
                                               MachineFunction &MF) const {

  unsigned Opc = I.getOpcode();

  if (Opc != TargetOpcode::G_STORE && Opc != TargetOpcode::G_LOAD)
    return false;

  const unsigned DefReg = I.getOperand(0).getReg();
  LLT Ty = MRI.getType(DefReg);
  const RegisterBank &RB = *RBI.getRegBank(DefReg, MRI, TRI);

  auto &MemOp = **I.memoperands_begin();
  if (MemOp.getOrdering() != AtomicOrdering::NotAtomic) {
    DEBUG(dbgs() << "Atomic load/store not supported yet\n");
    return false;
  }

  unsigned NewOpc = getLoadStoreOp(Ty, RB, Opc, MemOp.getAlignment());
  if (NewOpc == Opc)
    return false;

  X86AddressMode AM;
  X86SelectAddress(*MRI.getVRegDef(I.getOperand(1).getReg()), MRI, AM);

  I.setDesc(TII.get(NewOpc));
  MachineInstrBuilder MIB(MF, I);
  if (Opc == TargetOpcode::G_LOAD) {
    I.RemoveOperand(1);
    addFullAddress(MIB, AM);
  } else {
    // G_STORE (VAL, Addr), X86Store instruction (Addr, VAL)
    I.RemoveOperand(1);
    I.RemoveOperand(0);
    addFullAddress(MIB, AM).addUse(DefReg);
  }
  return constrainSelectedInstRegOperands(I, TII, TRI, RBI);
}

static unsigned getLeaOP(LLT Ty, const X86Subtarget &STI) {
  if (Ty == LLT::pointer(0, 64))
    return X86::LEA64r;
  else if (Ty == LLT::pointer(0, 32))
    return STI.isTarget64BitILP32() ? X86::LEA64_32r : X86::LEA32r;
  else
    llvm_unreachable("Can't get LEA opcode. Unsupported type.");
}

bool X86InstructionSelector::selectFrameIndexOrGep(MachineInstr &I,
                                                   MachineRegisterInfo &MRI,
                                                   MachineFunction &MF) const {
  unsigned Opc = I.getOpcode();

  if (Opc != TargetOpcode::G_FRAME_INDEX && Opc != TargetOpcode::G_GEP)
    return false;

  const unsigned DefReg = I.getOperand(0).getReg();
  LLT Ty = MRI.getType(DefReg);

  // Use LEA to calculate frame index and GEP
  unsigned NewOpc = getLeaOP(Ty, STI);
  I.setDesc(TII.get(NewOpc));
  MachineInstrBuilder MIB(MF, I);

  if (Opc == TargetOpcode::G_FRAME_INDEX) {
    addOffset(MIB, 0);
  } else {
    MachineOperand &InxOp = I.getOperand(2);
    I.addOperand(InxOp);        // set IndexReg
    InxOp.ChangeToImmediate(1); // set Scale
    MIB.addImm(0).addReg(0);
  }

  return constrainSelectedInstRegOperands(I, TII, TRI, RBI);
}

bool X86InstructionSelector::selectGlobalValue(MachineInstr &I,
                                               MachineRegisterInfo &MRI,
                                               MachineFunction &MF) const {
  unsigned Opc = I.getOpcode();

  if (Opc != TargetOpcode::G_GLOBAL_VALUE)
    return false;

  auto GV = I.getOperand(1).getGlobal();
  if (GV->isThreadLocal()) {
    return false; // TODO: we don't support TLS yet.
  }

  // Can't handle alternate code models yet.
  if (TM.getCodeModel() != CodeModel::Small)
    return 0;

  X86AddressMode AM;
  AM.GV = GV;
  AM.GVOpFlags = STI.classifyGlobalReference(GV);

  // TODO: The ABI requires an extra load. not supported yet.
  if (isGlobalStubReference(AM.GVOpFlags))
    return false;

  // TODO: This reference is relative to the pic base. not supported yet.
  if (isGlobalRelativeToPICBase(AM.GVOpFlags))
    return false;

  if (STI.isPICStyleRIPRel()) {
    // Use rip-relative addressing.
    assert(AM.Base.Reg == 0 && AM.IndexReg == 0);
    AM.Base.Reg = X86::RIP;
  }

  const unsigned DefReg = I.getOperand(0).getReg();
  LLT Ty = MRI.getType(DefReg);
  unsigned NewOpc = getLeaOP(Ty, STI);

  I.setDesc(TII.get(NewOpc));
  MachineInstrBuilder MIB(MF, I);

  I.RemoveOperand(1);
  addFullAddress(MIB, AM);

  return constrainSelectedInstRegOperands(I, TII, TRI, RBI);
}

bool X86InstructionSelector::selectConstant(MachineInstr &I,
                                            MachineRegisterInfo &MRI,
                                            MachineFunction &MF) const {
  if (I.getOpcode() != TargetOpcode::G_CONSTANT)
    return false;

  const unsigned DefReg = I.getOperand(0).getReg();
  LLT Ty = MRI.getType(DefReg);

  if (RBI.getRegBank(DefReg, MRI, TRI)->getID() != X86::GPRRegBankID)
    return false;

  uint64_t Val = 0;
  if (I.getOperand(1).isCImm()) {
    Val = I.getOperand(1).getCImm()->getZExtValue();
    I.getOperand(1).ChangeToImmediate(Val);
  } else if (I.getOperand(1).isImm()) {
    Val = I.getOperand(1).getImm();
  } else
    llvm_unreachable("Unsupported operand type.");

  unsigned NewOpc;
  switch (Ty.getSizeInBits()) {
  case 8:
    NewOpc = X86::MOV8ri;
    break;
  case 16:
    NewOpc = X86::MOV16ri;
    break;
  case 32:
    NewOpc = X86::MOV32ri;
    break;
  case 64: {
    // TODO: in case isUInt<32>(Val), X86::MOV32ri can be used
    if (isInt<32>(Val))
      NewOpc = X86::MOV64ri32;
    else
      NewOpc = X86::MOV64ri;
    break;
  }
  default:
    llvm_unreachable("Can't select G_CONSTANT, unsupported type.");
  }

  I.setDesc(TII.get(NewOpc));
  return constrainSelectedInstRegOperands(I, TII, TRI, RBI);
}

bool X86InstructionSelector::selectTrunc(MachineInstr &I,
                                         MachineRegisterInfo &MRI,
                                         MachineFunction &MF) const {
  if (I.getOpcode() != TargetOpcode::G_TRUNC)
    return false;

  const unsigned DstReg = I.getOperand(0).getReg();
  const unsigned SrcReg = I.getOperand(1).getReg();

  const LLT DstTy = MRI.getType(DstReg);
  const LLT SrcTy = MRI.getType(SrcReg);

  const RegisterBank &DstRB = *RBI.getRegBank(DstReg, MRI, TRI);
  const RegisterBank &SrcRB = *RBI.getRegBank(SrcReg, MRI, TRI);

  if (DstRB.getID() != SrcRB.getID()) {
    DEBUG(dbgs() << "G_TRUNC input/output on different banks\n");
    return false;
  }

  if (DstRB.getID() != X86::GPRRegBankID)
    return false;

  const TargetRegisterClass *DstRC = getRegClass(DstTy, DstRB);
  if (!DstRC)
    return false;

  const TargetRegisterClass *SrcRC = getRegClass(SrcTy, SrcRB);
  if (!SrcRC)
    return false;

  unsigned SubIdx;
  if (DstRC == SrcRC) {
    // Nothing to be done
    SubIdx = X86::NoSubRegister;
  } else if (DstRC == &X86::GR32RegClass) {
    SubIdx = X86::sub_32bit;
  } else if (DstRC == &X86::GR16RegClass) {
    SubIdx = X86::sub_16bit;
  } else if (DstRC == &X86::GR8RegClass) {
    SubIdx = X86::sub_8bit;
  } else {
    return false;
  }

  SrcRC = TRI.getSubClassWithSubReg(SrcRC, SubIdx);

  if (!RBI.constrainGenericRegister(SrcReg, *SrcRC, MRI) ||
      !RBI.constrainGenericRegister(DstReg, *DstRC, MRI)) {
    DEBUG(dbgs() << "Failed to constrain G_TRUNC\n");
    return false;
  }

  I.getOperand(1).setSubReg(SubIdx);

  I.setDesc(TII.get(X86::COPY));
  return true;
}

bool X86InstructionSelector::selectZext(MachineInstr &I,
                                        MachineRegisterInfo &MRI,
                                        MachineFunction &MF) const {
  if (I.getOpcode() != TargetOpcode::G_ZEXT)
    return false;

  const unsigned DstReg = I.getOperand(0).getReg();
  const unsigned SrcReg = I.getOperand(1).getReg();

  const LLT DstTy = MRI.getType(DstReg);
  const LLT SrcTy = MRI.getType(SrcReg);

  if (SrcTy != LLT::scalar(1))
    return false;

  unsigned AndOpc;
  if (DstTy == LLT::scalar(8))
    AndOpc = X86::AND8ri;
  else if (DstTy == LLT::scalar(16))
    AndOpc = X86::AND16ri8;
  else if (DstTy == LLT::scalar(32))
    AndOpc = X86::AND32ri8;
  else if (DstTy == LLT::scalar(64))
    AndOpc = X86::AND64ri8;
  else
    return false;

  unsigned DefReg = SrcReg;
  if (DstTy != LLT::scalar(8)) {
    DefReg = MRI.createVirtualRegister(getRegClass(DstTy, DstReg, MRI));
    BuildMI(*I.getParent(), I, I.getDebugLoc(),
            TII.get(TargetOpcode::SUBREG_TO_REG), DefReg)
        .addImm(0)
        .addReg(SrcReg)
        .addImm(X86::sub_8bit);
  }

  MachineInstr &AndInst =
      *BuildMI(*I.getParent(), I, I.getDebugLoc(), TII.get(AndOpc), DstReg)
           .addReg(DefReg)
           .addImm(1);

  constrainSelectedInstRegOperands(AndInst, TII, TRI, RBI);

  I.eraseFromParent();
  return true;
}

bool X86InstructionSelector::selectCmp(MachineInstr &I,
                                       MachineRegisterInfo &MRI,
                                       MachineFunction &MF) const {
  if (I.getOpcode() != TargetOpcode::G_ICMP)
    return false;

  X86::CondCode CC;
  bool SwapArgs;
  std::tie(CC, SwapArgs) = X86::getX86ConditionCode(
      (CmpInst::Predicate)I.getOperand(1).getPredicate());
  unsigned OpSet = X86::getSETFromCond(CC);

  unsigned LHS = I.getOperand(2).getReg();
  unsigned RHS = I.getOperand(3).getReg();

  if (SwapArgs)
    std::swap(LHS, RHS);

  unsigned OpCmp;
  LLT Ty = MRI.getType(LHS);

  switch (Ty.getSizeInBits()) {
  default:
    return false;
  case 8:
    OpCmp = X86::CMP8rr;
    break;
  case 16:
    OpCmp = X86::CMP16rr;
    break;
  case 32:
    OpCmp = X86::CMP32rr;
    break;
  case 64:
    OpCmp = X86::CMP64rr;
    break;
  }

  MachineInstr &CmpInst =
      *BuildMI(*I.getParent(), I, I.getDebugLoc(), TII.get(OpCmp))
           .addReg(LHS)
           .addReg(RHS);

  MachineInstr &SetInst = *BuildMI(*I.getParent(), I, I.getDebugLoc(),
                                   TII.get(OpSet), I.getOperand(0).getReg());

  constrainSelectedInstRegOperands(CmpInst, TII, TRI, RBI);
  constrainSelectedInstRegOperands(SetInst, TII, TRI, RBI);

  I.eraseFromParent();
  return true;
}

bool X86InstructionSelector::selectUadde(MachineInstr &I,
                                         MachineRegisterInfo &MRI,
                                         MachineFunction &MF) const {
  if (I.getOpcode() != TargetOpcode::G_UADDE)
    return false;

  const unsigned DstReg = I.getOperand(0).getReg();
  const unsigned CarryOutReg = I.getOperand(1).getReg();
  const unsigned Op0Reg = I.getOperand(2).getReg();
  const unsigned Op1Reg = I.getOperand(3).getReg();
  unsigned CarryInReg = I.getOperand(4).getReg();

  const LLT DstTy = MRI.getType(DstReg);

  if (DstTy != LLT::scalar(32))
    return false;

  // find CarryIn def instruction.
  MachineInstr *Def = MRI.getVRegDef(CarryInReg);
  while (Def->getOpcode() == TargetOpcode::G_TRUNC) {
    CarryInReg = Def->getOperand(1).getReg();
    Def = MRI.getVRegDef(CarryInReg);
  }

  unsigned Opcode;
  if (Def->getOpcode() == TargetOpcode::G_UADDE) {
    // carry set by prev ADD.

    BuildMI(*I.getParent(), I, I.getDebugLoc(), TII.get(X86::COPY), X86::EFLAGS)
        .addReg(CarryInReg);

    if (!RBI.constrainGenericRegister(CarryInReg, X86::GR32RegClass, MRI))
      return false;

    Opcode = X86::ADC32rr;
  } else if (auto val = getConstantVRegVal(CarryInReg, MRI)) {
    // carry is constant, support only 0.
    if (*val != 0)
      return false;

    Opcode = X86::ADD32rr;
  } else
    return false;

  MachineInstr &AddInst =
      *BuildMI(*I.getParent(), I, I.getDebugLoc(), TII.get(Opcode), DstReg)
           .addReg(Op0Reg)
           .addReg(Op1Reg);

  BuildMI(*I.getParent(), I, I.getDebugLoc(), TII.get(X86::COPY), CarryOutReg)
      .addReg(X86::EFLAGS);

  if (!constrainSelectedInstRegOperands(AddInst, TII, TRI, RBI) ||
      !RBI.constrainGenericRegister(CarryOutReg, X86::GR32RegClass, MRI))
    return false;

  I.eraseFromParent();
  return true;
}

bool X86InstructionSelector::selectExtract(MachineInstr &I,
                                           MachineRegisterInfo &MRI,
                                           MachineFunction &MF) const {

  if (I.getOpcode() != TargetOpcode::G_EXTRACT)
    return false;

  const unsigned DstReg = I.getOperand(0).getReg();
  const unsigned SrcReg = I.getOperand(1).getReg();
  int64_t Index = I.getOperand(2).getImm();

  const LLT DstTy = MRI.getType(DstReg);
  const LLT SrcTy = MRI.getType(SrcReg);

  // Meanwile handle vector type only.
  if (!DstTy.isVector())
    return false;

  if (Index % DstTy.getSizeInBits() != 0)
    return false; // Not extract subvector.

  if (Index == 0) {
    // Replace by extract subreg copy.
    if (!emitExtractSubreg(DstReg, SrcReg, I, MRI, MF))
      return false;

    I.eraseFromParent();
    return true;
  }

  bool HasAVX = STI.hasAVX();
  bool HasAVX512 = STI.hasAVX512();
  bool HasVLX = STI.hasVLX();

  if (SrcTy.getSizeInBits() == 256 && DstTy.getSizeInBits() == 128) {
    if (HasVLX)
      I.setDesc(TII.get(X86::VEXTRACTF32x4Z256rr));
    else if (HasAVX)
      I.setDesc(TII.get(X86::VEXTRACTF128rr));
    else
      return false;
  } else if (SrcTy.getSizeInBits() == 512 && HasAVX512) {
    if (DstTy.getSizeInBits() == 128)
      I.setDesc(TII.get(X86::VEXTRACTF32x4Zrr));
    else if (DstTy.getSizeInBits() == 256)
      I.setDesc(TII.get(X86::VEXTRACTF64x4Zrr));
    else
      return false;
  } else
    return false;

  // Convert to X86 VEXTRACT immediate.
  Index = Index / DstTy.getSizeInBits();
  I.getOperand(2).setImm(Index);

  return constrainSelectedInstRegOperands(I, TII, TRI, RBI);
}

bool X86InstructionSelector::emitExtractSubreg(unsigned DstReg, unsigned SrcReg,
                                               MachineInstr &I,
                                               MachineRegisterInfo &MRI,
                                               MachineFunction &MF) const {

  const LLT DstTy = MRI.getType(DstReg);
  const LLT SrcTy = MRI.getType(SrcReg);
  unsigned SubIdx = X86::NoSubRegister;

  if (!DstTy.isVector() || !SrcTy.isVector())
    return false;

  assert(SrcTy.getSizeInBits() > DstTy.getSizeInBits() &&
         "Incorrect Src/Dst register size");

  if (DstTy.getSizeInBits() == 128)
    SubIdx = X86::sub_xmm;
  else if (DstTy.getSizeInBits() == 256)
    SubIdx = X86::sub_ymm;
  else
    return false;

  const TargetRegisterClass *DstRC = getRegClass(DstTy, DstReg, MRI);
  const TargetRegisterClass *SrcRC = getRegClass(SrcTy, SrcReg, MRI);

  SrcRC = TRI.getSubClassWithSubReg(SrcRC, SubIdx);

  if (!RBI.constrainGenericRegister(SrcReg, *SrcRC, MRI) ||
      !RBI.constrainGenericRegister(DstReg, *DstRC, MRI)) {
    DEBUG(dbgs() << "Failed to constrain G_TRUNC\n");
    return false;
  }

  BuildMI(*I.getParent(), I, I.getDebugLoc(), TII.get(X86::COPY), DstReg)
      .addReg(SrcReg, 0, SubIdx);

  return true;
}

bool X86InstructionSelector::emitInsertSubreg(unsigned DstReg, unsigned SrcReg,
                                              MachineInstr &I,
                                              MachineRegisterInfo &MRI,
                                              MachineFunction &MF) const {

  const LLT DstTy = MRI.getType(DstReg);
  const LLT SrcTy = MRI.getType(SrcReg);
  unsigned SubIdx = X86::NoSubRegister;

  // TODO: support scalar types
  if (!DstTy.isVector() || !SrcTy.isVector())
    return false;

  assert(SrcTy.getSizeInBits() < DstTy.getSizeInBits() &&
         "Incorrect Src/Dst register size");

  if (SrcTy.getSizeInBits() == 128)
    SubIdx = X86::sub_xmm;
  else if (SrcTy.getSizeInBits() == 256)
    SubIdx = X86::sub_ymm;
  else
    return false;

  const TargetRegisterClass *SrcRC = getRegClass(SrcTy, SrcReg, MRI);
  const TargetRegisterClass *DstRC = getRegClass(DstTy, DstReg, MRI);

  if (!RBI.constrainGenericRegister(SrcReg, *SrcRC, MRI) ||
      !RBI.constrainGenericRegister(DstReg, *DstRC, MRI)) {
    DEBUG(dbgs() << "Failed to constrain INSERT_SUBREG\n");
    return false;
  }

  BuildMI(*I.getParent(), I, I.getDebugLoc(), TII.get(X86::COPY))
      .addReg(DstReg, RegState::DefineNoRead, SubIdx)
      .addReg(SrcReg);

  return true;
}

bool X86InstructionSelector::selectInsert(MachineInstr &I,
                                          MachineRegisterInfo &MRI,
                                          MachineFunction &MF) const {

  if (I.getOpcode() != TargetOpcode::G_INSERT)
    return false;

  const unsigned DstReg = I.getOperand(0).getReg();
  const unsigned SrcReg = I.getOperand(1).getReg();
  const unsigned InsertReg = I.getOperand(2).getReg();
  int64_t Index = I.getOperand(3).getImm();

  const LLT DstTy = MRI.getType(DstReg);
  const LLT InsertRegTy = MRI.getType(InsertReg);

  // Meanwile handle vector type only.
  if (!DstTy.isVector())
    return false;

  if (Index % InsertRegTy.getSizeInBits() != 0)
    return false; // Not insert subvector.

  if (Index == 0 && MRI.getVRegDef(SrcReg)->isImplicitDef()) {
    // Replace by subreg copy.
    if (!emitInsertSubreg(DstReg, InsertReg, I, MRI, MF))
      return false;

    I.eraseFromParent();
    return true;
  }

  bool HasAVX = STI.hasAVX();
  bool HasAVX512 = STI.hasAVX512();
  bool HasVLX = STI.hasVLX();

  if (DstTy.getSizeInBits() == 256 && InsertRegTy.getSizeInBits() == 128) {
    if (HasVLX)
      I.setDesc(TII.get(X86::VINSERTF32x4Z256rr));
    else if (HasAVX)
      I.setDesc(TII.get(X86::VINSERTF128rr));
    else
      return false;
  } else if (DstTy.getSizeInBits() == 512 && HasAVX512) {
    if (InsertRegTy.getSizeInBits() == 128)
      I.setDesc(TII.get(X86::VINSERTF32x4Zrr));
    else if (InsertRegTy.getSizeInBits() == 256)
      I.setDesc(TII.get(X86::VINSERTF64x4Zrr));
    else
      return false;
  } else
    return false;

  // Convert to X86 VINSERT immediate.
  Index = Index / InsertRegTy.getSizeInBits();

  I.getOperand(3).setImm(Index);

  return constrainSelectedInstRegOperands(I, TII, TRI, RBI);
}

bool X86InstructionSelector::selectUnmergeValues(MachineInstr &I,
                                                 MachineRegisterInfo &MRI,
                                                 MachineFunction &MF) const {
  if (I.getOpcode() != TargetOpcode::G_UNMERGE_VALUES)
    return false;

  // Split to extracts.
  unsigned NumDefs = I.getNumOperands() - 1;
  unsigned SrcReg = I.getOperand(NumDefs).getReg();
  unsigned DefSize = MRI.getType(I.getOperand(0).getReg()).getSizeInBits();

  for (unsigned Idx = 0; Idx < NumDefs; ++Idx) {

    MachineInstr &ExtrInst =
        *BuildMI(*I.getParent(), I, I.getDebugLoc(),
                 TII.get(TargetOpcode::G_EXTRACT), I.getOperand(Idx).getReg())
             .addReg(SrcReg)
             .addImm(Idx * DefSize);

    if (!select(ExtrInst))
      return false;
  }

  I.eraseFromParent();
  return true;
}

bool X86InstructionSelector::selectMergeValues(MachineInstr &I,
                                               MachineRegisterInfo &MRI,
                                               MachineFunction &MF) const {
  if (I.getOpcode() != TargetOpcode::G_MERGE_VALUES)
    return false;

  // Split to inserts.
  unsigned DstReg = I.getOperand(0).getReg();
  unsigned SrcReg0 = I.getOperand(1).getReg();

  const LLT DstTy = MRI.getType(DstReg);
  const LLT SrcTy = MRI.getType(SrcReg0);
  unsigned SrcSize = SrcTy.getSizeInBits();

  const RegisterBank &RegBank = *RBI.getRegBank(DstReg, MRI, TRI);

  // For the first src use insertSubReg.
  unsigned DefReg = MRI.createGenericVirtualRegister(DstTy);
  MRI.setRegBank(DefReg, RegBank);
  if (!emitInsertSubreg(DefReg, I.getOperand(1).getReg(), I, MRI, MF))
    return false;

  for (unsigned Idx = 2; Idx < I.getNumOperands(); ++Idx) {

    unsigned Tmp = MRI.createGenericVirtualRegister(DstTy);
    MRI.setRegBank(Tmp, RegBank);

    MachineInstr &InsertInst = *BuildMI(*I.getParent(), I, I.getDebugLoc(),
                                        TII.get(TargetOpcode::G_INSERT), Tmp)
                                    .addReg(DefReg)
                                    .addReg(I.getOperand(Idx).getReg())
                                    .addImm((Idx - 1) * SrcSize);

    DefReg = Tmp;

    if (!select(InsertInst))
      return false;
  }

  MachineInstr &CopyInst = *BuildMI(*I.getParent(), I, I.getDebugLoc(),
                                    TII.get(TargetOpcode::COPY), DstReg)
                                .addReg(DefReg);

  if (!select(CopyInst))
    return false;

  I.eraseFromParent();
  return true;
}
<<<<<<< HEAD
=======

bool X86InstructionSelector::selectCondBranch(MachineInstr &I,
                                              MachineRegisterInfo &MRI,
                                              MachineFunction &MF) const {
  if (I.getOpcode() != TargetOpcode::G_BRCOND)
    return false;

  const unsigned CondReg = I.getOperand(0).getReg();
  MachineBasicBlock *DestMBB = I.getOperand(1).getMBB();

  MachineInstr &TestInst =
      *BuildMI(*I.getParent(), I, I.getDebugLoc(), TII.get(X86::TEST8ri))
           .addReg(CondReg)
           .addImm(1);
  BuildMI(*I.getParent(), I, I.getDebugLoc(), TII.get(X86::JNE_1))
      .addMBB(DestMBB);

  constrainSelectedInstRegOperands(TestInst, TII, TRI, RBI);

  I.eraseFromParent();
  return true;
}

bool X86InstructionSelector::selectImplicitDefOrPHI(
    MachineInstr &I, MachineRegisterInfo &MRI) const {

  if (I.getOpcode() != TargetOpcode::G_IMPLICIT_DEF &&
      I.getOpcode() != TargetOpcode::G_PHI)
    return false;

  unsigned DstReg = I.getOperand(0).getReg();

  if (!MRI.getRegClassOrNull(DstReg)) {
    const LLT DstTy = MRI.getType(DstReg);
    const TargetRegisterClass *RC = getRegClass(DstTy, DstReg, MRI);

    if (!RBI.constrainGenericRegister(DstReg, *RC, MRI)) {
      DEBUG(dbgs() << "Failed to constrain " << TII.getName(I.getOpcode())
                   << " operand\n");
      return false;
    }
  }

  if (I.getOpcode() == TargetOpcode::G_IMPLICIT_DEF)
    I.setDesc(TII.get(X86::IMPLICIT_DEF));
  else
    I.setDesc(TII.get(X86::PHI));

  return true;
}

>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0
InstructionSelector *
llvm::createX86InstructionSelector(const X86TargetMachine &TM,
                                   X86Subtarget &Subtarget,
                                   X86RegisterBankInfo &RBI) {
  return new X86InstructionSelector(TM, Subtarget, RBI);
}
