// [C++ STL]
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

// [Styio]
#include "../StyioAST/AST.hpp"
#include "../StyioException/Exception.hpp"
#include "../StyioToken/Token.hpp"
#include "Util.hpp"

// [LLVM]
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/ExecutorProcessControl.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/Orc/Shared/ExecutorSymbolDef.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/LinkAllIR.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Utils.h"

void
im_here() {
  std::cout << "I am here!" << std::endl;
}

/*
  CreateEntryBlockAlloca: Create an alloca() instruction in the entry block of
  the function. This is used for mutable variables etc.
*/
llvm::AllocaInst*
StyioToLLVMIR::createAllocaFuncEntry(llvm::Function* TheFunction, llvm::StringRef VarName) {
  llvm::IRBuilder<> tmp_block(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  return tmp_block.CreateAlloca(
    llvm::Type::getDoubleTy(*theContext), /* Type should be modified in the future. */
    nullptr,
    VarName
  );
}

llvm::Type*
StyioToLLVMIR::matchType(std::string type) {
  if (type == "i32") {
    return theBuilder->getInt32Ty();
  }
  else if (type == "i64") {
    return theBuilder->getInt64Ty();
  }
  else if (type == "f32") {
    return theBuilder->getFloatTy();
  }
  else if (type == "f64") {
    return theBuilder->getDoubleTy();
  }

  else if (type == "i1") {
    return theBuilder->getInt1Ty();
  }
  else if (type == "i8") {
    return theBuilder->getInt8Ty();
  }
  else if (type == "i16") {
    return theBuilder->getInt16Ty();
  }
  else if (type == "i128") {
    return theBuilder->getInt128Ty();
  }

  return theBuilder->getInt1Ty();
}

void
StyioToLLVMIR::print_llvm_ir() {
  std::cout << "\n"
            << "\033[1;32mLLVM IR\033[0m"
            << "\n"
            << std::endl;

  /* llvm ir -> stdout */
  theModule->print(llvm::outs(), nullptr);
  /* llvm ir -> stderr */
  // llvm_module -> print(llvm::errs(), nullptr);
}

void
StyioToLLVMIR::print_test_results() {
  std::error_code EC;
  llvm::raw_fd_ostream output_stream(
    "output.ll",
    EC,
    llvm::sys::fs::OpenFlags::OF_None
  );
  if (EC) {
    std::cerr << "Can't open file output.ll; " << EC.message() << std::endl;
  }
  /* write to current_work_directory/output.ll */
  theModule->print(output_stream, nullptr);

  int lli_result = std::system("lli output.ll");

  string verifyModule_msg;

  if (llvm::verifyModule(*theModule)) {
    verifyModule_msg = "[\033[1;31mFAILED \033[0m] llvm::verifyModule()";
  }
  else {
    verifyModule_msg = "[\033[1;32mSUCCESS\033[0m] llvm::verifyModule()";
  }

  std::string lli_msg;
  if (lli_result) {
    lli_msg = "[\033[1;31mFAILED \033[0m] lli output.ll";
  }
  else {
    lli_msg = "[\033[1;32mSUCCESS\033[0m] lli output.ll";
  }

  /* print to stdout */
  std::cout << "\n"
            << verifyModule_msg
            << "\n"
            << lli_msg
            << std::endl;
}

/*
  StyioNaive
  [:] BoolAST
*/

llvm::Value*
StyioToLLVMIR::toLLVMIR(BoolAST* ast) {
  if (ast->getValue()) {
    return llvm::ConstantInt::getTrue(*theContext);
  }
  else {
    return theBuilder->getInt32(0);
  }
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(NoneAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(EOFAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(EmptyAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(PassAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(BreakAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(ReturnAST* ast) {
  return theBuilder->CreateRet(ast->getExpr()->toLLVMIR(this));
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(CommentAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(NameAST* ast) {
  auto output = theBuilder->getInt32(0);

  const string& varname = ast->getNameAsStr();

  if (named_values.contains(varname)) {
    return named_values[varname];
  }

  if (mut_vars.contains(varname)) {
    llvm::AllocaInst* variable = mut_vars[varname];

    return theBuilder->CreateLoad(variable->getAllocatedType(), variable);
    // return llvm_ir_builder->CreateLoad(variable->getAllocatedType(), variable, varname.c_str());
    // return variable; /* This line is WRONG! I keep it for debug. */
  }

  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(VarAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(ArgAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(OptArgAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(OptKwArgAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(VarTupleAST* ast) {
  auto output = theBuilder->getInt32(0);

  auto vars = ast->getParams();
  for (auto const& s : vars) {
    s->toLLVMIR(this);
  }

  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(DTypeAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(IntAST* ast) {
  switch (ast->getType()) {
    case StyioDataType::i1: {
      return theBuilder->getInt1(std::stoi(ast->getValue()));
    } break;

    case StyioDataType::i8: {
      return theBuilder->getInt8(std::stoi(ast->getValue()));
    } break;

    case StyioDataType::i16: {
      return theBuilder->getInt16(std::stoi(ast->getValue()));
    } break;

    case StyioDataType::i32: {
      return theBuilder->getInt32(std::stoi(ast->getValue()));
    } break;

    case StyioDataType::i64: {
      return theBuilder->getInt64(std::stoi(ast->getValue()));
    } break;

    case StyioDataType::i128: {
      return llvm::ConstantInt::get(llvm::Type::getInt128Ty(*theContext), std::stoi(ast->getValue()));
    } break;

    default: {
      return theBuilder->getInt32(std::stoi(ast->getValue()));
    } break;
  }
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(FloatAST* ast) {
  switch (ast->getType()) {
    case StyioDataType::f64:
      return llvm::ConstantFP::get(*theContext, llvm::APFloat(std::stod(ast->getValue())));

    default:
      return llvm::ConstantFP::get(*theContext, llvm::APFloat(std::stod(ast->getValue())));
  }
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(CharAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(StringAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(TypeConvertAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(FmtStrAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(ResourceAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(LocalPathAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(RemotePathAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(WebUrlAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(DBUrlAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(ExtPackAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(ListAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(TupleAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(SetAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(RangeAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(SizeOfAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(BinOpAST* ast) {
  llvm::Value* output = theBuilder->getInt32(0);

  llvm::Value* l_val = ast->getLhs()->toLLVMIR(this);
  llvm::Value* r_val = ast->getRhs()->toLLVMIR(this);

  switch (ast->getOperand()) {
    case StyioNodeHint::Bin_Add: {
      return theBuilder->CreateAdd(l_val, r_val);
    }

    break;

    case StyioNodeHint::Bin_Sub: {
      return theBuilder->CreateSub(l_val, r_val);
    }

    break;

    case StyioNodeHint::Bin_Mul: {
      return theBuilder->CreateMul(l_val, r_val);
    }

    break;

    case StyioNodeHint::Bin_Div: {
      // llvm_ir_builder -> CreateFDiv(l_val, r_val, "add");
    }

    break;

    case StyioNodeHint::Bin_Pow:
      // llvm_ir_builder -> CreateFAdd(l_val, r_val, "add");

      break;

    case StyioNodeHint::Bin_Mod:
      // llvm_ir_builder -> CreateFAdd(l_val, r_val, "add");

      break;

    case StyioNodeHint::Inc_Add:
      /* code */
      break;

    case StyioNodeHint::Inc_Sub:
      /* code */
      break;

    case StyioNodeHint::Inc_Mul:
      /* code */
      break;

    case StyioNodeHint::Inc_Div:
      /* code */
      break;

    default:
      break;
  }

  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(BinCompAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(CondAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(CallAST* ast) {
  auto output = theBuilder->getInt32(0);

  auto styio_func_args = ast->getArgList();

  llvm::Function* callee_func = theModule->getFunction(ast->getNameAsStr());

  if (callee_func == nullptr) {
    std::cout << "func " + ast->getNameAsStr() + " not found (as callee)" << std::endl;
    return output;
  }

  if (callee_func->arg_size() != styio_func_args.size()) {
    std::cout << "func " + ast->getNameAsStr() + " args not match" << std::endl;
    return output;
  }

  vector<llvm::Value*> llvm_args;
  for (size_t i = 0, e = styio_func_args.size(); i != e; ++i) {
    llvm_args.push_back(styio_func_args[i]->toLLVMIR(this));
    if (!llvm_args.back()) {
      std::cout << "ends here: !llvm_args.back()" << std::endl;
      return output;
    }
  }

  return theBuilder->CreateCall(callee_func, llvm_args);
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(ListOpAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

/*
  FlexBind

  Other Names For Search:
  - Flexible Binding
  - Mutable Variable
  - Mutable Assignment
*/
llvm::Value*
StyioToLLVMIR::toLLVMIR(FlexBindAST* ast) {
  auto output = theBuilder->getInt32(0);

  // // Look this variable up in the function.
  // llvm::AllocaInst* A = named_values[ast->getName()];
  // if (!A) {
  //   string errmsg = string("varname not found");
  //   throw StyioSyntaxError(errmsg);
  //   return nullptr;
  // }

  // // Load the value.
  // return llvm_ir_builder->CreateLoad(A->getAllocatedType(), A, Name.c_str());

  switch (ast->getValue()->hint()) {
    case StyioNodeHint::Int: {
      const string& varname = ast->getName();
      if (mut_vars.contains(varname)) {
        llvm::AllocaInst* variable = mut_vars[varname];
        theBuilder->CreateStore(ast->getValue()->toLLVMIR(this), variable);
      }
      else {
        llvm::AllocaInst* variable = theBuilder->CreateAlloca(
          theBuilder->getInt32Ty(),
          nullptr,
          varname.c_str()
        );
        mut_vars[varname] = variable;

        theBuilder->CreateStore(ast->getValue()->toLLVMIR(this), variable);
      }
    }

    break;

    case StyioNodeHint::Float: {
      const string& varname = ast->getName();
      if (mut_vars.contains(varname)) {
        llvm::AllocaInst* variable = mut_vars[varname];
        theBuilder->CreateStore(ast->getValue()->toLLVMIR(this), variable);
      }
      else {
        llvm::AllocaInst* variable = theBuilder->CreateAlloca(
          theBuilder->getDoubleTy(),
          nullptr,
          varname.c_str()
        );
        mut_vars[varname] = variable;

        theBuilder->CreateStore(ast->getValue()->toLLVMIR(this), variable);
      }
    }

    break;

    case StyioNodeHint::Id: {
      const string& varname = ast->getName();
      if (mut_vars.contains(varname)) {
        llvm::AllocaInst* variable = mut_vars[varname];
        theBuilder->CreateStore(ast->getValue()->toLLVMIR(this), variable);
      }
      else {
        llvm::AllocaInst* variable = theBuilder->CreateAlloca(theBuilder->getInt32Ty(), nullptr);
        mut_vars[varname] = variable;

        theBuilder->CreateStore(ast->getValue()->toLLVMIR(this), variable);
      }
    }

    break;

    case StyioNodeHint::BinOp: {
      const string& varname = ast->getName();
      if (mut_vars.contains(varname)) {
        llvm::AllocaInst* variable = mut_vars[varname];
        // llvm_ir_builder->CreateLoad(variable->getAllocatedType(), variable, varname.c_str());
        theBuilder->CreateStore(ast->getValue()->toLLVMIR(this), variable);
      }
      else {
        // llvm::AllocaInst* variable = llvm_ir_builder->CreateAlloca(llvm_ir_builder->getInt32Ty(), nullptr, varname);
        llvm::AllocaInst* variable = theBuilder->CreateAlloca(theBuilder->getInt32Ty(), nullptr);
        mut_vars[varname] = variable;

        theBuilder->CreateStore(ast->getValue()->toLLVMIR(this), variable);
      }
    }

    break;

    default:

      break;
  }

  // llvm_ir_builder->CreateAlloca(llvm::Type::getDoubleTy(*llvm_context), nullptr, ast->getName());

  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(FinalBindAST* ast) {
  auto output = theBuilder->getInt32(0);

  switch (ast->getValue()->hint()) {
    case StyioNodeHint::Int: {
      const string& varname = ast->getName();
      if (named_values.contains(varname)) {
        /* ERROR */
      }
      else {
        named_values[varname] = ast->getValue()->toLLVMIR(this);
      }
    } break;

    case StyioNodeHint::BinOp: {
      const string& varname = ast->getName();
      if (named_values.contains(varname)) {
        /* ERROR */
      }
      else {
        named_values[varname] = ast->getValue()->toLLVMIR(this);
      }
    }

    default: {
    } break;
  }

  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(StructAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(ReadFileAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(PrintAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(ForwardAST* ast) {
  auto output = theBuilder->getInt32(0);

  switch (ast->hint()) {
    case StyioNodeHint::Fill_Forward: {
      // ast -> getArgs() -> toLLVMIR(this);
      ast->getThen()->toLLVMIR(this);
    } break;

    case StyioNodeHint::Forward: {
      ast->getThen()->toLLVMIR(this);
    } break;

    default:
      break;
  }

  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(CheckEqAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(CheckIsinAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(FromToAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(InfiniteAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(AnonyFuncAST* ast) {
  auto output = theBuilder->getInt32(0);

  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(FuncAST* ast) {
  auto latest_insert_point = theBuilder->saveIP();

  if (ast->hasName()) {
    if (ast->hasArgs()) {
      if (ast->allArgsTyped()) {
        std::vector<llvm::Type*> llvm_func_args;
        for (auto& arg : ast->getAllArgs()) {
          llvm_func_args.push_back(arg->getLLVMType(this));
        }

        llvm::FunctionType* llvm_func_type = llvm::FunctionType::get(
          /* Result (Type) */ this->getLLVMType(ast),
          /* Params (Type) */ llvm_func_args,
          /* isVarArg */ false
        );

        llvm::Function* llvm_func =
          llvm::Function::Create(llvm_func_type, llvm::GlobalValue::ExternalLinkage, ast->getFuncName(), *theModule);

        for (size_t i = 0; i < llvm_func->arg_size(); i++) {
          llvm_func->getArg(i)->setName(ast->getAllArgs().at(i)->getName());
        }

        llvm::BasicBlock* block =
          llvm::BasicBlock::Create(*theContext, (ast->getFuncName() + "_entry"), llvm_func);

        theBuilder->SetInsertPoint(block);

        for (auto& arg : llvm_func->args()) {
          llvm::AllocaInst* alloca_inst = theBuilder->CreateAlloca(
            llvm::Type::getInt32Ty(*theContext),
            nullptr,
            arg.getName()
          );

          auto init_val = theBuilder->getInt32(0);

          theBuilder->CreateStore(&arg, alloca_inst);

          mut_vars[std::string(arg.getName())] = alloca_inst;
        }

        ast->getForward()->toLLVMIR(this);

        for (auto& arg : llvm_func->args()) {
          mut_vars.erase(std::string(arg.getName()));
        }

        llvm::verifyFunction(*llvm_func);
      }
      else {
        std::cout << "func " << ast->getFuncName() << " not all args typed" << std::endl;
      }
    }
    /* No Parameters */
    else {
      llvm::FunctionType* llvm_func_type = llvm::FunctionType::get(
        /* Result (Type) */ this->getLLVMType(ast),
        /* isVarArg */ false
      );
      llvm::Function* llvm_func = llvm::Function::Create(
        llvm_func_type,
        llvm::GlobalValue::ExternalLinkage,
        ast->getFuncName(),
        *theModule
      );

      llvm::BasicBlock* llvm_basic_block = llvm::BasicBlock::Create(*theContext, (ast->getFuncName() + "_entry"), llvm_func);

      theBuilder->SetInsertPoint(llvm_basic_block);

      ast->getForward()->toLLVMIR(this);

      llvm::verifyFunction(*llvm_func);
    }
  }

  theBuilder->restoreIP(latest_insert_point);

  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(IterAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(LoopAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(CondFlowAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(CasesAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(MatchCasesAST* ast) {
  auto output = theBuilder->getInt32(0);
  return output;
}

llvm::Value*
StyioToLLVMIR::toLLVMIR(BlockAST* ast) {
  auto output = theBuilder->getInt32(0);

  auto stmts = ast->getStmts();
  for (auto const& s : stmts) {
    s->toLLVMIR(this);
  }

  return output;
}

llvm::Function*
StyioToLLVMIR::toLLVMIR(MainBlockAST* ast) {
  /*
    Get Void Type: llvm::Type::getVoidTy(*llvm_context)
    Use Void Type: nullptr
  */
  llvm::FunctionType* func_type = llvm::FunctionType::get(theBuilder->getInt32Ty(), false);
  llvm::Function* main_func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, "main", *theModule);
  llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(*theContext, "main_entry", main_func);

  /* Add statements to the current basic block */
  theBuilder->SetInsertPoint(entry_block);

  auto stmts = ast->getStmts();
  for (auto const& s : stmts) {
    s->toLLVMIR(this);
  }

  // entry_block->getInstList()

  theBuilder->CreateRet(theBuilder->getInt32(0));

  return main_func;
  // return nullptr;
}

void
StyioToLLVMIR::execute() {
  auto RT = theORCJIT->getMainJITDylib().createResourceTracker();
  auto TSM = llvm::orc::ThreadSafeModule(std::move(theModule), std::move(theContext));
  llvm::ExitOnError exit_on_error;
  exit_on_error(theORCJIT->addModule(std::move(TSM), RT));

  // Look up the JIT'd code entry point.
  auto ExprSymbol = theORCJIT->lookup("main");
  if (!ExprSymbol) {
    std::cout << "not found" << std::endl;
    return;
  }

  std::cout << "after look up" << std::endl;

  // Cast the entry point address to a function pointer.
  int (*FP)() = ExprSymbol->getAddress().toPtr<int (*)()>();

  // Call into JIT'd code.
  std::cout << "result: " << FP() << std::endl;

  std::cout << "after run jit'd code" << std::endl;
}