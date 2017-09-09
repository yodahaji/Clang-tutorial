/***   CItutorial6.cpp   *****************************************************
 * This code is licensed under the New BSD license.
 * See LICENSE.txt for details.
 * 
 * The CI tutorials remake the original tutorials but using the
 * CompilerInstance object which has as one of its purpose to create commonly
 * used Clang types.
 *****************************************************************************/
#include <iostream>

#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/Support/raw_ostream.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Parse/ParseAST.h"
/***************************************************************************
 *This tutorial shows how to Recursively visit AST.It is seems to the version in
 *the official website,which also told how to recursive visit.
 **************************************************************************/

// RecursiveASTVisitor is is the big-kahuna visitor that traverses
// everything in the AST.
class MyRecursiveASTVisitor
    : public clang::RecursiveASTVisitor<MyRecursiveASTVisitor> {

 public:
  bool VisitTypedefDecl(clang::TypedefDecl *d);

};


bool MyRecursiveASTVisitor::VisitTypedefDecl(clang::TypedefDecl *d) {
  llvm::errs() << "Visiting " << d->getDeclKindName() << " "
               << d->getName() << "\n";
  return true; // returning false aborts the traversal
}


class MyASTConsumer : public clang::ASTConsumer {
 public:
    virtual bool HandleTopLevelDecl(clang::DeclGroupRef d);
};


bool MyASTConsumer::HandleTopLevelDecl(clang::DeclGroupRef d) {
  MyRecursiveASTVisitor rv;
  typedef clang::DeclGroupRef::iterator iter;
  for (iter b = d.begin(), e = d.end(); b != e; ++b) {
    rv.TraverseDecl(*b);
  }
  return true; // keep going
}


int main()
{
  using clang::CompilerInstance;
  using clang::TargetOptions;
  using clang::TargetInfo;
  using clang::FileEntry;

  //create compiler instance and complex initialization
  CompilerInstance ci; 
  ci.createDiagnostics();
  ci.createFileManager();
  ci.createSourceManager(ci.getFileManager());


  //set the target
    std::shared_ptr<clang::TargetOptions> pto = std::make_shared<clang::TargetOptions>();
  pto->Triple = llvm::sys::getDefaultTargetTriple();
    TargetInfo *pti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(), pto);
    ci.setTarget(pti);

  //create Preprocessor
  ci.createPreprocessor(clang::TU_Complete);

  //create AST environment
  ci.setASTConsumer(llvm::make_unique<MyASTConsumer>());
  ci.createASTContext();

  //get the file pointer and set its mainFileId
  const FileEntry *pFile = ci.getFileManager().getFile("test.c");
    ci.getSourceManager().setMainFileID( ci.getSourceManager().createFileID( pFile, clang::SourceLocation(), clang::SrcMgr::C_User));
  
  //Start to process the file
  ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                           &ci.getPreprocessor());
  //Parse The AST                                           
  clang::ParseAST(ci.getPreprocessor(), &ci.getASTConsumer(), ci.getASTContext());
  //End to process the file
  ci.getDiagnosticClient().EndSourceFile();

  return 0;
}
