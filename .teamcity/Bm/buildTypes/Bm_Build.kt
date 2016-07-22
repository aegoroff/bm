package Bm.buildTypes

import jetbrains.buildServer.configs.kotlin.v10.*
import jetbrains.buildServer.configs.kotlin.v10.BuildStep
import jetbrains.buildServer.configs.kotlin.v10.BuildStep.*
import jetbrains.buildServer.configs.kotlin.v10.buildSteps.ExecBuildStep
import jetbrains.buildServer.configs.kotlin.v10.buildSteps.ExecBuildStep.*
import jetbrains.buildServer.configs.kotlin.v10.buildSteps.exec
import jetbrains.buildServer.configs.kotlin.v10.triggers.VcsTrigger
import jetbrains.buildServer.configs.kotlin.v10.triggers.VcsTrigger.*
import jetbrains.buildServer.configs.kotlin.v10.triggers.vcs

object Bm_Build : BuildType({
    uuid = "9e7c50ed-e270-4113-a707-ece290844921"
    extId = "Bm_Build"
    name = "Build"

    artifactRules = """bin\Release"""

    vcs {
        root(Bm.vcsRoots.Bm_HttpsGithubComAegoroffBmRefsHeadsMaster)

        cleanCheckout = true
    }

    steps {
        step {
            type = "jetbrains-cmake-conf"
            param("ui-jetbrains-cmake-conf-cmake-build-type", "Default")
            param("ui-jetbrains-cmake-conf-cmake-command", """"C:/Program Files (x86)/JetBrains/CLion 2016.1.3/bin/cmake/bin/cmake.exe"""")
            param("ui-jetbrains-cmake-conf-debug-mode", "true")
            param("ui-jetbrains-cmake-conf-developer-warnings", "false")
            param("ui-jetbrains-cmake-conf-makefile-generator", "Visual Studio 14 2015 Win64")
            param("ui-jetbrains-cmake-conf-no-warn-unused-cli", "true")
            param("ui-jetbrains-cmake-conf-print-trace", "true")
            param("ui-jetbrains-cmake-conf-source-path", "CMakeLists.txt")
        }
        step {
            type = "jetbrains-cmake-build"
            param("ui-jetbrains-cmake-build-build-target", "ALL_BUILD")
            param("ui-jetbrains-cmake-build-clean-before-build", "true")
            param("ui-jetbrains-cmake-build-cmake-build-configuration", "Release")
            param("ui-jetbrains-cmake-build-cmake-command", """"C:/Program Files (x86)/JetBrains/CLion 2016.1.3/bin/cmake/bin/cmake.exe"""")
        }
        exec {
            path = """bin\Release\bmt.exe"""
        }
    }

    triggers {
        vcs {
        }
    }
})
