//
//  loader.c
//  ParasiteLoader
//
//  Created by Alexander Zielenski on 3/30/16.
//  Copyright © 2016 ParasiteTeam. All rights reserved.
//

#include "main.h"
#include "loader.h"

const CFStringRef kOPFiltersKey     = CFSTR("OPFilters");
const CFStringRef kSIMBLFiltersKey  = CFSTR("SIMBLTargetApplications");
const char *OPLibrariesPath         = "/Library/Opee/Extensions";

const CFStringRef kOPBundleIdentifierKey = CFSTR("BundleIdentifier");
const CFStringRef kOPMinBundleVersionKey = CFSTR("MinBundleVersion");
const CFStringRef kOPMaxBundleVersionKey = CFSTR("MaxBundleVersion");

const CFStringRef kOPCoreFoundationVersionKey = CFSTR("CoreFoundationVersion");
const CFStringRef kOPModeKey                  = CFSTR("Mode");
const CFStringRef kOPAnyValue                 = CFSTR("Any");
const CFStringRef kOPExecutablesKey           = CFSTR("Executables");
const CFStringRef kOPBundlesKey               = CFSTR("Bundles");
const CFStringRef kOPClassesKey               = CFSTR("Classes");

__attribute__((constructor))
static void __ParasiteInit(int argc, char **argv, char **envp) {
    if (dlopen("/System/Library/Frameworks/Security.framework/Security", RTLD_LAZY | RTLD_NOLOAD) == NULL)
        return;
    
    if (argc < 1 || argv == NULL || getuid() == 0)
        return;
    
    
    // The first argument is the spawned process
    // Get the process name by looking at the last path
    // component.
    char *executable = strrchr(argv[0], '/');
    executable = (executable == NULL) ? argv[0] : executable + 1;

    CFBundleRef mainBundle = CFBundleGetMainBundle();
    
#ifdef DEBUG
//    CFDictionaryRef info = CFBundleGetInfoDictionary(mainBundle);
//    CFStringRef identifier = (info == NULL) ? NULL : CFDictionaryGetValue(info, kCFBundleIdentifierKey);
//    OPLog(OPLogLevelNotice, "Installing %@ [%s] (%.2f)", identifier, executable, kCFCoreFoundationVersionNumber);
#endif
    
    CFStringRef executableName = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault,
                                                                 executable,
                                                                 kCFStringEncodingUTF8,
                                                                 kCFAllocatorNull);
    
    // Process extensions for all users
    CFURLRef libraries = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault,
                                                                 (const UInt8 *)OPLibrariesPath,
                                                                 strlen(OPLibrariesPath),
                                                                 true);
    
    if (access(OPLibrariesPath, X_OK | R_OK) == -1) {
//        OPLog(OPLogLevelError, "Unable to access root libraries directory");
        
    } else if (libraries != NULL) {
        __ParasiteProcessExtensions(libraries, mainBundle, executableName);
    }
    
    CFRelease(executableName);
    CFRelease(libraries);
    
}