#include "Utils.h"
#include "Domain.h"
#include <sstream>

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
  // print 2 maps side by side
  llvm::outs() << variable(ins) << "\n";
  int inWidth = 5;
  std::vector<std::string> lines(std::max(inMap.size(), outMap.size()));
  int iLines = 0;
  for (auto &fact : inMap) {
    std::ostringstream oss;
    oss << fact.first << " |-> " << fact.second;
    auto line = oss.str();
    inWidth = std::max(inWidth, (int)line.size());
    lines[iLines++] = std::move(line);
  }

  iLines = 0;
  for (auto &fact : outMap) {
    std::ostringstream oss;
    oss << " | " << fact.first << " |-> " << fact.second;
    auto line = oss.str();
    lines[iLines].resize(inWidth, ' ');
    lines[iLines].insert(lines[iLines].end(), line.begin(), line.end());
    ++iLines;
  }
  std::string header = "IN";
  header.resize(inWidth, ' ');
  llvm::outs() << header << " | OUT\n";
  for (auto &line : lines) {
    llvm::outs() << line << "\n";
  }
  llvm::outs() << "\n";
}

} // namespace dataflow