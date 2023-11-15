import sys
import os
import git
import json

def getFileLastCommenter(repoPath,filePath):
    repo = git.Repo(repoPath)
    cms = repo.iter_commits(None,filePath)
    lastestCommit = None
    for c in cms:
        if lastestCommit == None or lastestCommit.committed_datetime < c.committed_datetime:
            lastestCommit = c


def getConfigRepo(cfgPath,level):

    outPath = cfgPath
    i = level
    if not os.path.isdir(outPath):
        outPath = os.path.dirname(outPath)
        i = i - 1
    while i > 0:
        outPath = os.path.abspath(os.path.dirname(outPath) + os.path.sep + ".")
        i -= 1

    return outPath

reqType = sys.argv[1]
if reqType == "author":
    absCfgPath = os.path.abspath(sys.argv[2])
    repoPath = getConfigRepo(absCfgPath,3)
    getFileLastCommenter(repoPath,absCfgPath)
