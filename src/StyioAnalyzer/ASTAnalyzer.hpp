#pragma once
#ifndef STYIO_AST_ANALYZER_VISITOR_H_
#define STYIO_AST_ANALYZER_VISITOR_H_

// [STL]
#include <iostream>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

// [Styio]
#include "../StyioAST/ASTDecl.hpp"
#include "../StyioIR/IRDecl.hpp"

// Generic Visitor
template <typename... Types>
class AnalyzerVisitor;

template <typename T>
class AnalyzerVisitor<T>
{
public:
  virtual void typeInfer(T* t) = 0;

  virtual StyioIR* toStyioIR(T* t) = 0;
};

template <typename T, typename... Types>
class AnalyzerVisitor<T, Types...> : public AnalyzerVisitor<Types...>
{
public:
  using AnalyzerVisitor<Types...>::typeInfer;
  using AnalyzerVisitor<Types...>::toStyioIR;

  virtual void typeInfer(T* t) = 0;

  virtual StyioIR* toStyioIR(T* t) = 0;
};

using StyioAnalyzerVisitor = AnalyzerVisitor<
  class CommentAST,

  class NoneAST,
  class EmptyAST,

  class BoolAST,
  class IntAST,
  class FloatAST,
  class CharAST,

  class StringAST,
  class SetAST,
  class ListAST,

  class StructAST,
  class TupleAST,

  class NameAST,
  class DTypeAST,

  class VarAST,
  class ArgAST,
  class OptArgAST,
  class OptKwArgAST,

  class FlexBindAST,
  class FinalBindAST,

  class BinCompAST,
  class CondAST,
  class BinOpAST,

  class AnonyFuncAST,
  class FuncAST,

  class CallAST,
  class AttrAST,

  class SizeOfAST,
  class TypeConvertAST,
  class ListOpAST,
  class RangeAST,

  class IterAST,
  class LoopAST,

  class CondFlowAST,

  class EOFAST,
  class PassAST,
  class BreakAST,
  class ReturnAST,

  class CasesAST,
  class MatchCasesAST,

  class BlockAST,
  class MainBlockAST,

  class ExtPackAST,

  class InfiniteAST,

  class VarTupleAST,

  class ForwardAST,
  class CheckEqAST,
  class CheckIsinAST,
  class FromToAST,

  class CODPAST,

  class FmtStrAST,

  class ResourceAST,

  class LocalPathAST,
  class RemotePathAST,
  class WebUrlAST,
  class DBUrlAST,

  class PrintAST,
  class ReadFileAST>;

class StyioAnalyzer : public StyioAnalyzerVisitor
{
  unordered_map<string, FuncAST*> func_defs;

public:
  StyioAnalyzer() {}

  ~StyioAnalyzer() {}

  /* Styio AST Type Inference */

  void typeInfer(BoolAST* ast);
  void typeInfer(NoneAST* ast);
  void typeInfer(EOFAST* ast);
  void typeInfer(EmptyAST* ast);
  void typeInfer(PassAST* ast);
  void typeInfer(BreakAST* ast);
  void typeInfer(ReturnAST* ast);
  void typeInfer(CommentAST* ast);
  void typeInfer(NameAST* ast);
  void typeInfer(VarAST* ast);
  void typeInfer(ArgAST* ast);
  void typeInfer(OptArgAST* ast);
  void typeInfer(OptKwArgAST* ast);
  void typeInfer(VarTupleAST* ast);
  void typeInfer(DTypeAST* ast);
  void typeInfer(IntAST* ast);
  void typeInfer(FloatAST* ast);
  void typeInfer(CharAST* ast);
  void typeInfer(StringAST* ast);
  void typeInfer(TypeConvertAST* ast);
  void typeInfer(FmtStrAST* ast);
  void typeInfer(LocalPathAST* ast);
  void typeInfer(RemotePathAST* ast);
  void typeInfer(WebUrlAST* ast);
  void typeInfer(DBUrlAST* ast);
  void typeInfer(ListAST* ast);
  void typeInfer(TupleAST* ast);
  void typeInfer(SetAST* ast);
  void typeInfer(RangeAST* ast);
  void typeInfer(SizeOfAST* ast);
  void typeInfer(BinOpAST* ast);
  void typeInfer(BinCompAST* ast);
  void typeInfer(CondAST* ast);
  void typeInfer(CallAST* ast);
  void typeInfer(AttrAST* ast);
  void typeInfer(ListOpAST* ast);
  void typeInfer(ResourceAST* ast);
  void typeInfer(FlexBindAST* ast);
  void typeInfer(FinalBindAST* ast);
  void typeInfer(StructAST* ast);
  void typeInfer(ReadFileAST* ast);
  void typeInfer(PrintAST* ast);
  void typeInfer(ExtPackAST* ast);
  void typeInfer(BlockAST* ast);
  void typeInfer(CasesAST* ast);
  void typeInfer(CondFlowAST* ast);
  void typeInfer(CheckEqAST* ast);
  void typeInfer(CheckIsinAST* ast);
  void typeInfer(FromToAST* ast);
  void typeInfer(ForwardAST* ast);
  void typeInfer(CODPAST* ast);
  void typeInfer(InfiniteAST* ast);
  void typeInfer(AnonyFuncAST* ast);
  void typeInfer(FuncAST* ast);
  void typeInfer(LoopAST* ast);
  void typeInfer(IterAST* ast);
  void typeInfer(MatchCasesAST* ast);
  void typeInfer(MainBlockAST* ast);

  StyioIR* toStyioIR(BoolAST* ast);
  StyioIR* toStyioIR(NoneAST* ast);
  StyioIR* toStyioIR(EOFAST* ast);
  StyioIR* toStyioIR(EmptyAST* ast);
  StyioIR* toStyioIR(PassAST* ast);
  StyioIR* toStyioIR(BreakAST* ast);
  StyioIR* toStyioIR(ReturnAST* ast);
  StyioIR* toStyioIR(CommentAST* ast);
  StyioIR* toStyioIR(NameAST* ast);
  StyioIR* toStyioIR(VarAST* ast);
  StyioIR* toStyioIR(ArgAST* ast);
  StyioIR* toStyioIR(OptArgAST* ast);
  StyioIR* toStyioIR(OptKwArgAST* ast);
  StyioIR* toStyioIR(VarTupleAST* ast);
  StyioIR* toStyioIR(DTypeAST* ast);
  StyioIR* toStyioIR(IntAST* ast);
  StyioIR* toStyioIR(FloatAST* ast);
  StyioIR* toStyioIR(CharAST* ast);
  StyioIR* toStyioIR(StringAST* ast);
  StyioIR* toStyioIR(TypeConvertAST* ast);
  StyioIR* toStyioIR(FmtStrAST* ast);
  StyioIR* toStyioIR(LocalPathAST* ast);
  StyioIR* toStyioIR(RemotePathAST* ast);
  StyioIR* toStyioIR(WebUrlAST* ast);
  StyioIR* toStyioIR(DBUrlAST* ast);
  StyioIR* toStyioIR(ListAST* ast);
  StyioIR* toStyioIR(TupleAST* ast);
  StyioIR* toStyioIR(SetAST* ast);
  StyioIR* toStyioIR(RangeAST* ast);
  StyioIR* toStyioIR(SizeOfAST* ast);
  StyioIR* toStyioIR(BinOpAST* ast);
  StyioIR* toStyioIR(BinCompAST* ast);
  StyioIR* toStyioIR(CondAST* ast);
  StyioIR* toStyioIR(CallAST* ast);
  StyioIR* toStyioIR(AttrAST* ast);
  StyioIR* toStyioIR(ListOpAST* ast);
  StyioIR* toStyioIR(ResourceAST* ast);
  StyioIR* toStyioIR(FlexBindAST* ast);
  StyioIR* toStyioIR(FinalBindAST* ast);
  StyioIR* toStyioIR(StructAST* ast);
  StyioIR* toStyioIR(ReadFileAST* ast);
  StyioIR* toStyioIR(PrintAST* ast);
  StyioIR* toStyioIR(ExtPackAST* ast);
  StyioIR* toStyioIR(BlockAST* ast);
  StyioIR* toStyioIR(CasesAST* ast);
  StyioIR* toStyioIR(CondFlowAST* ast);
  StyioIR* toStyioIR(CheckEqAST* ast);
  StyioIR* toStyioIR(CheckIsinAST* ast);
  StyioIR* toStyioIR(FromToAST* ast);
  StyioIR* toStyioIR(ForwardAST* ast);
  StyioIR* toStyioIR(CODPAST* ast);
  StyioIR* toStyioIR(InfiniteAST* ast);
  StyioIR* toStyioIR(AnonyFuncAST* ast);
  StyioIR* toStyioIR(FuncAST* ast);
  StyioIR* toStyioIR(LoopAST* ast);
  StyioIR* toStyioIR(IterAST* ast);
  StyioIR* toStyioIR(MatchCasesAST* ast);
  StyioIR* toStyioIR(MainBlockAST* ast);
};

#endif