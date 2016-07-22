package Bm.vcsRoots

import jetbrains.buildServer.configs.kotlin.v10.*
import jetbrains.buildServer.configs.kotlin.v10.vcs.GitVcsRoot

object Bm_HttpsGithubComAegoroffBmRefsHeadsMaster : GitVcsRoot({
    uuid = "c5979391-1338-4e39-8116-9e83e9b3a43a"
    extId = "Bm_HttpsGithubComAegoroffBmRefsHeadsMaster"
    name = "https://github.com/aegoroff/bm#refs/heads/master"
    url = "https://github.com/aegoroff/bm"
    authMethod = password {
        userName = "aegoroff"
        password = "zxxad071d8cdbe74ea794f0f9fb680cc28a6d205d774a448d35cb4687deaa3d0d4026225b15f4c6f23a775d03cbe80d301b"
    }
})
