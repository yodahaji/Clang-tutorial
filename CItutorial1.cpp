/***   CItutorial1.cpp   *****************************************************
 * This code is licensed under the New BSD license.
 * See LICENSE.txt for details.
 * 
 * The _CI tutorials remake the original tutorials but using the
 * CompilerInstance object which has as one of its purpose to create commonly
 * used Clang types.
 *****************************************************************************/
#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"

/******************************************************************************
 * This tutorial just shows off the steps needed to build up to a Preprocessor
 * object. Note that the order below is important.
 *****************************************************************************/
int main()
{
    /*****************************************************************
    *The CompilerInstance manages various obeject what is need to
    *run the compiler.e.g,the diagnostic engine,the preprocessor,etc.
    *And it also provides utility routines for constructing and
    *manipulating the common Clang objects.
    *****************************************************************/
    using clang::CompilerInstance;
    /*****************************************************************
    *Options for controlling the target.For example,it controls what
    *ABI to use,what the cpu to generate code for ,and other features.
    *****************************************************************/
    using clang::TargetOptions;
    /*****************************************************************
    *Exposes information about the current target. 
    *For example,the platform names,validate input/output size,
    *big endian or little endian....
    *****************************************************************/
    using clang::TargetInfo;
    
    CompilerInstance ci;    
    //create DiagnosticsEngine      
    //DiagnosticsEngine is central concept of the Diagnostic Subsystem,
    //which is an important part of how the compiler communicates 
    //with the human. Diagnostics are the warnings and errors produced
    //when the code is incorrect or dubious.               
    ci.createDiagnostics();            
    //create FileManager 
    //FileManager:Implements support for file system lookup, 
    //file system caching, and directory search management.           
    ci.createFileManager();
    //create SourceManager
    //SourceManager handles loading and caching of source files into memory.
    //This object owns the MemoryBuffer objects for all of the loaded files 
    //and assigns unique FileID's for each unique #include chain.
    ci.createSourceManager(ci.getFileManager());

    //get the targetInfo
 	std::shared_ptr<TargetOptions> pTargetOptions = std::make_shared<TargetOptions>();
	pTargetOptions->Triple = llvm::sys::getDefaultTargetTriple();
	TargetInfo *pTargetInfo = TargetInfo::CreateTargetInfo(ci.getDiagnostics(), pTargetOptions);
    ci.setTarget(pTargetInfo);
    //create Preprocessor
    //Preprocessor engages in a tight little dance with the lexer 
    //to efficiently preprocess tokens. 
    
    //Argument describes the kind of translation unit being processed.
    //TU_Complete means the translation unit is a complete translation unit.
    //TU_Prefix,The translation unit is a prefix to a translation unit, and is
    //not complete.
    //TU_Module The translation unit is a module.
    ci.createPreprocessor(clang::TU_Complete);
    
    return 0;
}
