#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <regex>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

using namespace llvm;

namespace {
struct GlobalVariablePass : public ModulePass {
  static char ID;
  GlobalVariablePass() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    LLVMContext &Context = M.getContext();
    IRBuilder<> Builder(Context);

    Function *GetEnv = cast<Function>(
        M.getOrInsertFunction("getenv",
                              PointerType::getUnqual(Type::getInt8Ty(Context)),
                              PointerType::getUnqual(Type::getInt8Ty(Context)))
            .getCallee());

    Function *Fprintf = cast<Function>(
        M.getOrInsertFunction(
             "fprintf", Type::getInt32Ty(Context),
             PointerType::getUnqual(Type::getInt8Ty(Context)), // FILE*
             PointerType::getUnqual(Type::getInt8Ty(Context)), // const char*
             nullptr)
            .getCallee());

    Function *Fopen = cast<Function>(
        M.getOrInsertFunction("fopen",
                              PointerType::getUnqual(Type::getInt8Ty(Context)),
                              PointerType::getUnqual(Type::getInt8Ty(Context)),
                              PointerType::getUnqual(Type::getInt8Ty(Context)))
            .getCallee());

    Function *Fclose = cast<Function>(
        M.getOrInsertFunction("fclose", Type::getInt32Ty(Context),
                              PointerType::getUnqual(Type::getInt8Ty(Context)))
            .getCallee());

    // Read environment variable UNI_KLEE_SYMBOLIC_GLOBALS_FILE
    const char *EnvVarName = "UNI_KLEE_SYMBOLIC_GLOBALS_FILE";
    const char *GlobalsFilePath = std::getenv(EnvVarName);
    if (!GlobalsFilePath) {
      errs() << "Environment variable " << EnvVarName << " is not set.\n";
      return false;
    }

    // Read target variables from the specified file
    std::ifstream GlobalsFile(GlobalsFilePath);
    if (!GlobalsFile.is_open()) {
      errs() << "Failed to open globals file: " << GlobalsFilePath << "\n";
      return false;
    }

    std::map<std::string, std::string> TargetVariables;
    std::string Line;
    std::regex lineRegex(R"(\[array\s+([\w\.]+)\]\s+\[name\s+([\w\.]+)\])"); 
    while (std::getline(GlobalsFile, Line)) {
      if (!Line.empty()) {
        std::smatch match;
        if (std::regex_search(Line, match, lineRegex) && match.size() > 2) {
          TargetVariables[match.str(2)] = match.str(1);
          errs() << "Found target variable: " << match.str(2) << "=" << match.str(1) << "\n";
        }
      }
    }
    GlobalsFile.close();

    bool Modified = false;

    // Find the "uni_klee_heap_check" function
    Function *HeapCheckFunc = M.getFunction("uni_klee_heap_check");
    if (!HeapCheckFunc) {
      errs() << "Function uni_klee_heap_check not found.\n";
      return false;
    }

    // Insert instrumentation at the beginning of "uni_klee_heap_check"
    BasicBlock &EntryBlock = HeapCheckFunc->getEntryBlock();
    Builder.SetInsertPoint(&EntryBlock, EntryBlock.begin());

    for (GlobalVariable &GV : M.globals()) {
      if (!GV.hasName())
        continue;

      std::string VarName = GV.getName().str();

      if (TargetVariables.count(VarName) > 0) {
        errs() << "Found target variable: " << VarName << "\n";
        std::string symName = TargetVariables[VarName];

        // getenv("UNI_KLEE_MEM_RESULT")
        Value *EnvVar = Builder.CreateGlobalStringPtr("UNI_KLEE_MEM_RESULT");
        Value *FilePath = Builder.CreateCall(GetEnv, {EnvVar});

        // fopen(result_file, "a")
        Value *AppendMode = Builder.CreateGlobalStringPtr("a");
        Value *FileHandle = Builder.CreateCall(Fopen, {FilePath, AppendMode});

        // fprintf(f, "Variable: %s = %d\n", VarName, GV)
        Value *FormatStr = Builder.CreateGlobalStringPtr("[global] [sym \n");
        Value *VarNameStr = Builder.CreateGlobalStringPtr(VarName);
        Value *VarValue = Builder.CreateLoad(GV.getValueType(), &GV);

        Builder.CreateCall(Fprintf,
                           {FileHandle, FormatStr, VarNameStr, VarValue});

        // fclose(f)
        Builder.CreateCall(Fclose, {FileHandle});

        Modified = true;
      }
    }

    return Modified;
  }
};
} // namespace

char GlobalVariablePass::ID = 0;
static RegisterPass<GlobalVariablePass> X("global-var-pass",
                                          "Track Global Variables Pass");
