//===- IRTransformLayer.h - Run all IR through a functor --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Run all IR passed in through a user supplied functor.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_EXECUTIONENGINE_ORC_IRTRANSFORMLAYER_H
#define LLVM_EXECUTIONENGINE_ORC_IRTRANSFORMLAYER_H

#include "llvm/ExecutionEngine/JITSymbol.h"
#include <memory>
#include <string>

namespace llvm {
class Module;
namespace orc {

/// @brief IR mutating layer.
///
///   This layer applies a user supplied transform to each module that is added,
/// then adds the transformed module to the layer below.
template <typename BaseLayerT, typename TransformFtor>
class IRTransformLayer {
public:

  /// @brief Handle to a set of added modules.
  using ModuleHandleT = typename BaseLayerT::ModuleHandleT;

  /// @brief Construct an IRTransformLayer with the given BaseLayer
  IRTransformLayer(BaseLayerT &BaseLayer,
                   TransformFtor Transform = TransformFtor())
    : BaseLayer(BaseLayer), Transform(std::move(Transform)) {}

  /// @brief Apply the transform functor to the module, then add the module to
  ///        the layer below, along with the memory manager and symbol resolver.
  ///
  /// @return A handle for the added modules.
<<<<<<< HEAD
  ModuleHandleT addModule(std::shared_ptr<Module> M,
                          std::shared_ptr<JITSymbolResolver> Resolver) {
=======
  Expected<ModuleHandleT>
  addModule(std::shared_ptr<Module> M,
            std::shared_ptr<JITSymbolResolver> Resolver) {
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0
    return BaseLayer.addModule(Transform(std::move(M)), std::move(Resolver));
  }

  /// @brief Remove the module associated with the handle H.
<<<<<<< HEAD
  void removeModule(ModuleHandleT H) { BaseLayer.removeModule(H); }
=======
  Error removeModule(ModuleHandleT H) { return BaseLayer.removeModule(H); }
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0

  /// @brief Search for the given named symbol.
  /// @param Name The name of the symbol to search for.
  /// @param ExportedSymbolsOnly If true, search only for exported symbols.
  /// @return A handle for the given named symbol, if it exists.
  JITSymbol findSymbol(const std::string &Name, bool ExportedSymbolsOnly) {
    return BaseLayer.findSymbol(Name, ExportedSymbolsOnly);
  }

  /// @brief Get the address of the given symbol in the context of the module
  ///        represented by the handle H. This call is forwarded to the base
  ///        layer's implementation.
  /// @param H The handle for the module to search in.
  /// @param Name The name of the symbol to search for.
  /// @param ExportedSymbolsOnly If true, search only for exported symbols.
  /// @return A handle for the given named symbol, if it is found in the
  ///         given module.
  JITSymbol findSymbolIn(ModuleHandleT H, const std::string &Name,
                         bool ExportedSymbolsOnly) {
    return BaseLayer.findSymbolIn(H, Name, ExportedSymbolsOnly);
  }

  /// @brief Immediately emit and finalize the module represented by the given
  ///        handle.
  /// @param H Handle for module to emit/finalize.
<<<<<<< HEAD
  void emitAndFinalize(ModuleHandleT H) {
    BaseLayer.emitAndFinalize(H);
=======
  Error emitAndFinalize(ModuleHandleT H) {
    return BaseLayer.emitAndFinalize(H);
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0
  }

  /// @brief Access the transform functor directly.
  TransformFtor& getTransform() { return Transform; }

  /// @brief Access the mumate functor directly.
  const TransformFtor& getTransform() const { return Transform; }

private:
  BaseLayerT &BaseLayer;
  TransformFtor Transform;
};

} // end namespace orc
} // end namespace llvm

#endif // LLVM_EXECUTIONENGINE_ORC_IRTRANSFORMLAYER_H
