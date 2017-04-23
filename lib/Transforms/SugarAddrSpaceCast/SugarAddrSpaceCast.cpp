//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file transforms invalid addrSpaceCast intruction between AMDGCN non-generic
// address space into a valid one by using generic addresspace (4) as bridge.
//
//===----------------------------------------------------------------------===//

#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"

using namespace llvm;

namespace {

// New AMDGCN generic now is AS0 (previously AS4)
enum AddressSpace {
  ADDRESS_SPACE_GENERIC = 0,
  ADDRESS_SPACE_GLOBAL = 1,
  ADDRESS_SPACE_SHARED = 3,
  ADDRESS_SPACE_CONST = 4,
  ADDRESS_SPACE_PRIVATE = 5,

  ADDRESS_SPACE_PARAM = 101
};

using IRBuilderTy = llvm::IRBuilder<>;

//
// Invalid addrspacecast between non-generic is not allowed in AMDGCN
//
// This cast
//   %1 = addrspacecast i32 addrspace(1)* %0 to i32 addrspace(5)*
//
// will be transformed to two casts
//   %1 = addrspacecast i32 addrspace(1)* %0 to i32*
//   %2 = addrspacecast i32* %1 to i32 addrspace(5)*
//
class SugarAddrSpaceCast : public FunctionPass {

  class TargetFinder : public InstVisitor<TargetFinder> {
  private:
    Function* F;
    Module* M;
    SmallVector<Instruction*, 32> WorkList;

  public:
    TargetFinder(Function* Func) : F(Func) { M = F->getParent(); }

    // Specially handle PtrToInt instruction
    //  from:
    //     %146 = sub i32 %145, ptrtoint ([4194304 x i8]* addrspacecast ([4194304 x i8] addrspace(1)* @gpuHeap to [4194304 x i8]*) to i32)
    //  to:
    //     %h1 = ptrtoint ([4194304 x i8] addrspace(1)* @gpuHeap to i32)
    //     %146 =  sub i32 %145, %h1
    //
    // Note that, the following casts are not valid for AMDGCN target. Not sure why!
    //  to:
    //     %h = addrspacecast [4194304 x i8] addrspace(1)* @gpuHeap to [4194304 x i8]*
    //     %h1 = ptrtoint ([4194304 x i8]* %h to i32)
    //     %146 =  sub i32 %145, %h1
    void HandlePtrToIntInst(Instruction* I) {
      // Sub
      for (unsigned Idx = 0;
          Idx < I->getNumOperands() && 1/*I->getOpcode() == Instruction::Sub*/;
          ++Idx)
      {
        Value* V = I->getOperand(Idx);
        // Sub-expression of Sub
        if (auto *CE = dyn_cast<ConstantExpr>(V)) {
          // prtoint
          if (CE->getOpcode() == Instruction::PtrToInt) {
            if (auto *CE2 = dyn_cast<ConstantExpr>(CE->getOperand(0))) {
              // addrspacecast
              Instruction* I2 = CE->getAsInstruction();
              if (CE2->getOpcode() == Instruction::AddrSpaceCast) {
                I2->insertBefore(I);
                I2->setOperand(0, CE2->getOperand(0));
                I->setOperand(Idx, I2);
                return;
              }
            }
          }
        }
      }
    }

    void run() {
      // Step 1: Translate ConstantExpr into instruction
      for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
        HandlePtrToIntInst(I.operator->());

      // Step 2: Now walk through all instructions
      for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
        visit(*I);

      // Don't run before any inferring
      while(!WorkList.empty()) {
        Instruction* I = WorkList.pop_back_val();
        if (!dyn_cast<AddrSpaceCastInst>(I)) continue;

        Type * DestType = I->getType();
        PointerType * DestPtrType = dyn_cast<PointerType>(DestType);
        if (!DestPtrType) return;

        // In amdgcn, cast between non-generic addrspace is not allowed.
        // Use generic as bridge
        int AS_target = DestPtrType->getAddressSpace();
        Type* OpTy = I->getOperand(0)->getType();
        PointerType* OpPtrTy = dyn_cast<PointerType>(OpTy);
        if (!OpPtrTy) return;

        int AS_source = OpPtrTy->getAddressSpace();
        if (AS_source == ADDRESS_SPACE_GENERIC || AS_target == ADDRESS_SPACE_GENERIC)
          return;

        IRBuilderTy IRB(I);

        // Convert to generic addrspace
        PointerType* GenericTy = OpPtrTy->getPointerElementType()->getPointerTo(ADDRESS_SPACE_GENERIC);
        Value* GenericValue = IRB.CreatePointerCast(I->getOperand(0), GenericTy);

        // Convert to dest addrspace
        Value* DestValue = IRB.CreatePointerCast(GenericValue, DestType);
        I->replaceAllUsesWith(DestValue);
        I->eraseFromParent();
      }
    }

    void visitAddrSpaceCastInst(AddrSpaceCastInst &I) {
      WorkList.push_back(&I);
    }
  };

public:
  static char ID;
  SugarAddrSpaceCast() : FunctionPass(ID) { }
  ~SugarAddrSpaceCast() { }

  void getAnalysisUsage(AnalysisUsage& AU) const {
    AU.addRequired<CallGraphWrapperPass>();
  }

  bool runOnFunction(Function& F) {
    if (!(F.getCallingConv() == llvm::CallingConv::AMDGPU_KERNEL))
      return false;

    TargetFinder finder(&F);
    finder.run();

    return true;
  }
};

} // namespace

char SugarAddrSpaceCast::ID = 0;
static RegisterPass<SugarAddrSpaceCast>
Y("sugar-addrspacecast", "Transform invalid addrsSpaceCast between non-generic");
