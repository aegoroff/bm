package Bm

import Bm.buildTypes.*
import Bm.vcsRoots.*
import Bm.vcsRoots.Bm_HttpsGithubComAegoroffBmRefsHeadsMaster
import jetbrains.buildServer.configs.kotlin.v10.*
import jetbrains.buildServer.configs.kotlin.v10.Project
import jetbrains.buildServer.configs.kotlin.v10.ProjectFeature
import jetbrains.buildServer.configs.kotlin.v10.ProjectFeature.*
import jetbrains.buildServer.configs.kotlin.v10.projectFeatures.VersionedSettings
import jetbrains.buildServer.configs.kotlin.v10.projectFeatures.VersionedSettings.*
import jetbrains.buildServer.configs.kotlin.v10.projectFeatures.versionedSettings

object Project : Project({
    uuid = "e285bef6-02a5-4ce1-9160-609b4ae34272"
    extId = "Bm"
    parentId = "_Root"
    name = "BM"

    vcsRoot(Bm_HttpsGithubComAegoroffBmRefsHeadsMaster)

    buildType(Bm_Build)

    features {
        versionedSettings {
            id = "PROJECT_EXT_11"
            mode = VersionedSettings.Mode.ENABLED
            buildSettingsMode = VersionedSettings.BuildSettingsMode.USE_CURRENT_SETTINGS
            rootExtId = Bm_HttpsGithubComAegoroffBmRefsHeadsMaster.extId
            showChanges = true
            settingsFormat = VersionedSettings.Format.KOTLIN
        }
        feature {
            id = "PROJECT_EXT_9"
            type = "IssueTracker"
            param("authType", "accesstoken")
            param("name", "aegoroff/bm")
            param("pattern", """#(\d+)""")
            param("repository", "https://github.com/aegoroff/bm")
            param("secure:accessToken", "zxxeea5006848a5c400f241f8d9d0fa084c97c7da0b9c4162570226a567bee6c8e2")
            param("secure:password", "zxx775d03cbe80d301b")
            param("type", "GithubIssues")
            param("username", "")
        }
    }
})
