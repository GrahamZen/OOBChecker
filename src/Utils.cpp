#include "Utils.h"
#include "Domain.h"

const char *WHITESPACES = " \t\n\r";
const size_t VARIABLE_PADDED_LEN = 8;

namespace dataflow {

std::string variable(const llvm::Value *val) {
  std::string code;
  llvm::raw_string_ostream ss(code);
  val->print(ss);
  code.erase(0, code.find_first_not_of(WHITESPACES));
  auto ret = code.substr(0, code.find_first_of(WHITESPACES));
  if (ret == "ret" || ret == "br" || ret == "store") {
    return code;
  }
  if (ret == "i1" || ret == "i8" || ret == "i32" || ret == "i64") {
    ret = code;
  }
  for (auto i = ret.size(); i < VARIABLE_PADDED_LEN; i++) {
    ret += " ";
  }
  return ret;
}

std::string address(const llvm::Value *val) {
  std::string code;
  llvm::raw_string_ostream ss(code);
  val->print(ss);
  code.erase(0, code.find_first_not_of(WHITESPACES));
  code = "@(" + code + ")";
  return code;
}

IntervalDomain getOrExtract(const FactMap& facts, const llvm::Value *val) {
  auto key = variable(val);
  if (facts.count(key)) {
    return facts.at(key);
  } else {
    return IntervalDomain { val };
  }
}

void printMap(const llvm::Function &func, const InsFactMap &inMap, const InsFactMap &outMap) {
  llvm::outs() << "Dataflow Analysis Results:\n";
  for (auto iter = llvm::inst_begin(func), end = llvm::inst_end(func); iter != end; ++iter) {
    auto ins = &(*iter);
    llvm::outs() << "Instruction: " << *ins << "\n";
    llvm::outs() << "In set: \n";
    llvm::outs() << inMap.at(ins) << "\n";
    llvm::outs() << "Out set: \n";
    llvm::outs() << outMap.at(ins) << "\n";
    llvm::outs() << "\n";
  }
}

void printInstructionTransfer(const llvm::Instruction *ins, const FactMap& inMap,
                              const FactMap& outMap) {
  llvm::outs() << variable(ins) << "\nIN =====\n" << inMap << "\nOUT =====\n" << outMap << "\n";
}

} // namespace dataflow