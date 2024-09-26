#include <llvm-c/Core.h>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"

using namespace llvm;

typedef enum
{
    LLVMZludaAtomicRMWBinOpXchg,     /**< Set the new value and return the one old */
    LLVMZludaAtomicRMWBinOpAdd,      /**< Add a value and return the old one */
    LLVMZludaAtomicRMWBinOpSub,      /**< Subtract a value and return the old one */
    LLVMZludaAtomicRMWBinOpAnd,      /**< And a value and return the old one */
    LLVMZludaAtomicRMWBinOpNand,     /**< Not-And a value and return the old one */
    LLVMZludaAtomicRMWBinOpOr,       /**< OR a value and return the old one */
    LLVMZludaAtomicRMWBinOpXor,      /**< Xor a value and return the old one */
    LLVMZludaAtomicRMWBinOpMax,      /**< Sets the value if it's greater than the
                                  original using a signed comparison and return
                                  the old one */
    LLVMZludaAtomicRMWBinOpMin,      /**< Sets the value if it's Smaller than the
                                  original using a signed comparison and return
                                  the old one */
    LLVMZludaAtomicRMWBinOpUMax,     /**< Sets the value if it's greater than the
                                 original using an unsigned comparison and return
                                 the old one */
    LLVMZludaAtomicRMWBinOpUMin,     /**< Sets the value if it's greater than the
                                  original using an unsigned comparison and return
                                  the old one */
    LLVMZludaAtomicRMWBinOpFAdd,     /**< Add a floating point value and return the
                                  old one */
    LLVMZludaAtomicRMWBinOpFSub,     /**< Subtract a floating point value and return the
                                old one */
    LLVMZludaAtomicRMWBinOpFMax,     /**< Sets the value if it's greater than the
                                 original using an floating point comparison and
                                 return the old one */
    LLVMZludaAtomicRMWBinOpFMin,     /**< Sets the value if it's smaller than the
                                 original using an floating point comparison and
                                 return the old one */
    LLVMZludaAtomicRMWBinOpUIncWrap, /**< Increments the value, wrapping back to zero
                                 when incremented above input value */
    LLVMZludaAtomicRMWBinOpUDecWrap, /**< Decrements the value, wrapping back to
                                 the input value when decremented below zero */
} LLVMZludaAtomicRMWBinOp;

static llvm::AtomicRMWInst::BinOp mapFromLLVMRMWBinOp(LLVMZludaAtomicRMWBinOp BinOp)
{
    switch (BinOp)
    {
    case LLVMZludaAtomicRMWBinOpXchg:
        return llvm::AtomicRMWInst::Xchg;
    case LLVMZludaAtomicRMWBinOpAdd:
        return llvm::AtomicRMWInst::Add;
    case LLVMZludaAtomicRMWBinOpSub:
        return llvm::AtomicRMWInst::Sub;
    case LLVMZludaAtomicRMWBinOpAnd:
        return llvm::AtomicRMWInst::And;
    case LLVMZludaAtomicRMWBinOpNand:
        return llvm::AtomicRMWInst::Nand;
    case LLVMZludaAtomicRMWBinOpOr:
        return llvm::AtomicRMWInst::Or;
    case LLVMZludaAtomicRMWBinOpXor:
        return llvm::AtomicRMWInst::Xor;
    case LLVMZludaAtomicRMWBinOpMax:
        return llvm::AtomicRMWInst::Max;
    case LLVMZludaAtomicRMWBinOpMin:
        return llvm::AtomicRMWInst::Min;
    case LLVMZludaAtomicRMWBinOpUMax:
        return llvm::AtomicRMWInst::UMax;
    case LLVMZludaAtomicRMWBinOpUMin:
        return llvm::AtomicRMWInst::UMin;
    case LLVMZludaAtomicRMWBinOpFAdd:
        return llvm::AtomicRMWInst::FAdd;
    case LLVMZludaAtomicRMWBinOpFSub:
        return llvm::AtomicRMWInst::FSub;
    case LLVMZludaAtomicRMWBinOpFMax:
        return llvm::AtomicRMWInst::FMax;
    case LLVMZludaAtomicRMWBinOpFMin:
        return llvm::AtomicRMWInst::FMin;
    case LLVMZludaAtomicRMWBinOpUIncWrap:
        return llvm::AtomicRMWInst::UIncWrap;
    case LLVMZludaAtomicRMWBinOpUDecWrap:
        return llvm::AtomicRMWInst::UDecWrap;
    }

    llvm_unreachable("Invalid LLVMZludaAtomicRMWBinOp value!");
}

static AtomicOrdering mapFromLLVMOrdering(LLVMAtomicOrdering Ordering)
{
    switch (Ordering)
    {
    case LLVMAtomicOrderingNotAtomic:
        return AtomicOrdering::NotAtomic;
    case LLVMAtomicOrderingUnordered:
        return AtomicOrdering::Unordered;
    case LLVMAtomicOrderingMonotonic:
        return AtomicOrdering::Monotonic;
    case LLVMAtomicOrderingAcquire:
        return AtomicOrdering::Acquire;
    case LLVMAtomicOrderingRelease:
        return AtomicOrdering::Release;
    case LLVMAtomicOrderingAcquireRelease:
        return AtomicOrdering::AcquireRelease;
    case LLVMAtomicOrderingSequentiallyConsistent:
        return AtomicOrdering::SequentiallyConsistent;
    }

    llvm_unreachable("Invalid LLVMAtomicOrdering value!");
}

LLVM_C_EXTERN_C_BEGIN

LLVMValueRef LLVMZludaBuildAlloca(LLVMBuilderRef B, LLVMTypeRef Ty, unsigned AddrSpace,
                                  const char *Name)
{
    return llvm::wrap(llvm::unwrap(B)->CreateAlloca(llvm::unwrap(Ty), AddrSpace, nullptr, Name));
}

LLVMValueRef LLVMZludaBuildAtomicRMW(LLVMBuilderRef B, LLVMZludaAtomicRMWBinOp op,
                                     LLVMValueRef PTR, LLVMValueRef Val,
                                     char *scope,
                                     LLVMAtomicOrdering ordering)
{
    auto builder = llvm::unwrap(B);
    LLVMContext &context = builder->getContext();
    llvm::AtomicRMWInst::BinOp intop = mapFromLLVMRMWBinOp(op);
    return llvm::wrap(builder->CreateAtomicRMW(
        intop, llvm::unwrap(PTR), llvm::unwrap(Val), llvm::MaybeAlign(),
        mapFromLLVMOrdering(ordering),
        context.getOrInsertSyncScopeID(scope)));
}

LLVM_C_EXTERN_C_END