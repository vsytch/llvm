//===-- llvm/CodeGen/GlobalISel/Legalizer.cpp -----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
/// \file This file implements the LegalizerHelper class to legalize individual
/// instructions and the LegalizePass wrapper pass for the primary
/// legalization.
//
//===----------------------------------------------------------------------===//

#include "llvm/CodeGen/GlobalISel/Legalizer.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/CodeGen/GlobalISel/LegalizerCombiner.h"
#include "llvm/CodeGen/GlobalISel/LegalizerHelper.h"
#include "llvm/CodeGen/GlobalISel/Utils.h"
#include "llvm/CodeGen/MachineOptimizationRemarkEmitter.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/Debug.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Target/TargetSubtargetInfo.h"

#include <iterator>

#define DEBUG_TYPE "legalizer"

using namespace llvm;

char Legalizer::ID = 0;
INITIALIZE_PASS_BEGIN(Legalizer, DEBUG_TYPE,
                      "Legalize the Machine IR a function's Machine IR", false,
                      false)
INITIALIZE_PASS_DEPENDENCY(TargetPassConfig)
INITIALIZE_PASS_END(Legalizer, DEBUG_TYPE,
                    "Legalize the Machine IR a function's Machine IR", false,
                    false)

Legalizer::Legalizer() : MachineFunctionPass(ID) {
  initializeLegalizerPass(*PassRegistry::getPassRegistry());
}

void Legalizer::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<TargetPassConfig>();
  MachineFunctionPass::getAnalysisUsage(AU);
}

void Legalizer::init(MachineFunction &MF) {
}

<<<<<<< HEAD
bool Legalizer::combineMerges(MachineInstr &MI, MachineRegisterInfo &MRI,
                              const TargetInstrInfo &TII,
                              MachineIRBuilder &MIRBuilder) {
  if (MI.getOpcode() != TargetOpcode::G_UNMERGE_VALUES)
    return false;

  unsigned NumDefs = MI.getNumOperands() - 1;
  unsigned SrcReg = MI.getOperand(NumDefs).getReg();
  MachineInstr &MergeI = *MRI.def_instr_begin(SrcReg);
  if (MergeI.getOpcode() != TargetOpcode::G_MERGE_VALUES)
    return false;

  const unsigned NumMergeRegs = MergeI.getNumOperands() - 1;

  if (NumMergeRegs < NumDefs) {
    if (NumDefs % NumMergeRegs != 0)
      return false;

    MIRBuilder.setInstr(MI);
    // Transform to UNMERGEs, for example
    //   %1 = G_MERGE_VALUES %4, %5
    //   %9, %10, %11, %12 = G_UNMERGE_VALUES %1
    // to
    //   %9, %10 = G_UNMERGE_VALUES %4
    //   %11, %12 = G_UNMERGE_VALUES %5

    const unsigned NewNumDefs = NumDefs / NumMergeRegs;
    for (unsigned Idx = 0; Idx < NumMergeRegs; ++Idx) {
      SmallVector<unsigned, 2> DstRegs;
      for (unsigned j = 0, DefIdx = Idx * NewNumDefs; j < NewNumDefs;
           ++j, ++DefIdx)
        DstRegs.push_back(MI.getOperand(DefIdx).getReg());

      MIRBuilder.buildUnmerge(DstRegs, MergeI.getOperand(Idx + 1).getReg());
    }

  } else if (NumMergeRegs > NumDefs) {
    if (NumMergeRegs % NumDefs != 0)
      return false;

    MIRBuilder.setInstr(MI);
    // Transform to MERGEs
    //   %6 = G_MERGE_VALUES %17, %18, %19, %20
    //   %7, %8 = G_UNMERGE_VALUES %6
    // to
    //   %7 = G_MERGE_VALUES %17, %18
    //   %8 = G_MERGE_VALUES %19, %20

    const unsigned NumRegs = NumMergeRegs / NumDefs;
    for (unsigned DefIdx = 0; DefIdx < NumDefs; ++DefIdx) {
      SmallVector<unsigned, 2> Regs;
      for (unsigned j = 0, Idx = NumRegs * DefIdx + 1; j < NumRegs; ++j, ++Idx)
        Regs.push_back(MergeI.getOperand(Idx).getReg());

      MIRBuilder.buildMerge(MI.getOperand(DefIdx).getReg(), Regs);
    }

  } else {
    // FIXME: is a COPY appropriate if the types mismatch? We know both
    // registers are allocatable by now.
    if (MRI.getType(MI.getOperand(0).getReg()) !=
        MRI.getType(MergeI.getOperand(1).getReg()))
      return false;

    for (unsigned Idx = 0; Idx < NumDefs; ++Idx)
      MRI.replaceRegWith(MI.getOperand(Idx).getReg(),
                         MergeI.getOperand(Idx + 1).getReg());
  }

  MI.eraseFromParent();
  if (MRI.use_empty(MergeI.getOperand(0).getReg()))
    MergeI.eraseFromParent();
  return true;
}

=======
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0
bool Legalizer::runOnMachineFunction(MachineFunction &MF) {
  // If the ISel pipeline failed, do not bother running that pass.
  if (MF.getProperties().hasProperty(
          MachineFunctionProperties::Property::FailedISel))
    return false;
  DEBUG(dbgs() << "Legalize Machine IR for: " << MF.getName() << '\n');
  init(MF);
  const TargetPassConfig &TPC = getAnalysis<TargetPassConfig>();
  MachineOptimizationRemarkEmitter MORE(MF, /*MBFI=*/nullptr);
  LegalizerHelper Helper(MF);

  // FIXME: an instruction may need more than one pass before it is legal. For
  // example on most architectures <3 x i3> is doubly-illegal. It would
  // typically proceed along a path like: <3 x i3> -> <3 x i8> -> <8 x i8>. We
  // probably want a worklist of instructions rather than naive iterate until
  // convergence for performance reasons.
  bool Changed = false;
  MachineBasicBlock::iterator NextMI;
  for (auto &MBB : MF) {
    for (auto MI = MBB.begin(); MI != MBB.end(); MI = NextMI) {
      // Get the next Instruction before we try to legalize, because there's a
      // good chance MI will be deleted.
      NextMI = std::next(MI);

      // Only legalize pre-isel generic instructions: others don't have types
      // and are assumed to be legal.
      if (!isPreISelGenericOpcode(MI->getOpcode()))
        continue;
      unsigned NumNewInsns = 0;
      using VecType = SetVector<MachineInstr *, SmallVector<MachineInstr *, 8>>;
      VecType WorkList;
      VecType CombineList;
      Helper.MIRBuilder.recordInsertions([&](MachineInstr *MI) {
        // Only legalize pre-isel generic instructions.
        // Legalization process could generate Target specific pseudo
        // instructions with generic types. Don't record them
        if (isPreISelGenericOpcode(MI->getOpcode())) {
          ++NumNewInsns;
          WorkList.insert(MI);
          CombineList.insert(MI);
        }
      });
      WorkList.insert(&*MI);
      LegalizerCombiner C(Helper.MIRBuilder, MF.getRegInfo());
      bool Changed = false;
      LegalizerHelper::LegalizeResult Res;
      do {
        assert(!WorkList.empty() && "Expecting illegal ops");
        while (!WorkList.empty()) {
          NumNewInsns = 0;
          MachineInstr *CurrInst = WorkList.pop_back_val();
          Res = Helper.legalizeInstrStep(*CurrInst);
          // Error out if we couldn't legalize this instruction. We may want to
          // fall back to DAG ISel instead in the future.
          if (Res == LegalizerHelper::UnableToLegalize) {
            Helper.MIRBuilder.stopRecordingInsertions();
            if (Res == LegalizerHelper::UnableToLegalize) {
              reportGISelFailure(MF, TPC, MORE, "gisel-legalize",
                                 "unable to legalize instruction", *CurrInst);
              return false;
            }
          }
          Changed |= Res == LegalizerHelper::Legalized;
          // If CurrInst was legalized, there's a good chance that it might have
          // been erased. So remove it from the Combine List.
          if (Res == LegalizerHelper::Legalized)
            CombineList.remove(CurrInst);

#ifndef NDEBUG
          if (NumNewInsns)
            for (unsigned I = WorkList.size() - NumNewInsns,
                          E = WorkList.size();
                 I != E; ++I)
              DEBUG(dbgs() << ".. .. New MI: " << *WorkList[I];);
#endif
        }
        // Do the combines.
        while (!CombineList.empty()) {
          NumNewInsns = 0;
          MachineInstr *CurrInst = CombineList.pop_back_val();
          SmallVector<MachineInstr *, 4> DeadInstructions;
          Changed |= C.tryCombineInstruction(*CurrInst, DeadInstructions);
          for (auto *DeadMI : DeadInstructions) {
            DEBUG(dbgs() << ".. Erasing Dead Instruction " << *DeadMI);
            CombineList.remove(DeadMI);
            WorkList.remove(DeadMI);
            DeadMI->eraseFromParent();
          }
#ifndef NDEBUG
          if (NumNewInsns)
            for (unsigned I = CombineList.size() - NumNewInsns,
                          E = CombineList.size();
                 I != E; ++I)
              DEBUG(dbgs() << ".. .. Combine New MI: " << *CombineList[I];);
#endif
        }
      } while (!WorkList.empty());

      Helper.MIRBuilder.stopRecordingInsertions();
    }
  }

  MachineRegisterInfo &MRI = MF.getRegInfo();
  MachineIRBuilder MIRBuilder(MF);
  LegalizerCombiner C(MIRBuilder, MRI);
  for (auto &MBB : MF) {
    for (auto MI = MBB.begin(); MI != MBB.end(); MI = NextMI) {
      // Get the next Instruction before we try to legalize, because there's a
      // good chance MI will be deleted.
      // TOOD: Perhaps move this to a combiner pass later?.
      NextMI = std::next(MI);
<<<<<<< HEAD
      Changed |= combineMerges(*MI, MRI, TII, Helper.MIRBuilder);
=======
      SmallVector<MachineInstr *, 4> DeadInsts;
      Changed |= C.tryCombineMerges(*MI, DeadInsts);
      for (auto *DeadMI : DeadInsts)
        DeadMI->eraseFromParent();
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0
    }
  }

  return Changed;
}
