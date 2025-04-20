# MCP Server Setup and Usage

This document provides instructions for setting up and using the MCP server in this project.

## Prerequisites

Make sure you have the following installed on your system:

- WSL
- Node.js
  - linux/WSL
    - Install Node.js and npm using your package manager, e.g.:
      ```bash
      sudo apt update
      sudo apt install -y nodejs npm
      ```
  - Windows:
    - Install Node.js and npm by downloading the installer (.msi) from https://nodejs.org/en/download
- TypeScript compiler (`tsc`)
- Required system packages

## Installation

### Linux / WSL

```bash
cd mcp
npm install
npm run build
```

### Windows

```powershell
cd mcp
npm install
npm run buildwin
```

## MCP Clients Settings

Add or update the following entries in your setting.json of the clients to configure the MCP servers:
- For **claude**, the configuration file is located at:
  `C:\Users\your-user-name\AppData\Roaming\Claude\claude_desktop_config.json`
- For **cline** by WSL, the configuration file is located at:
  `/home/your-user-name/.vscode-server/data/User/globalStorage/saoudrizwan.claude-dev/settings/cline_mcp_settings.json`

### case of MCP server on WSL
```json
{
  "mcpServers": {
    "yabms": {
      "command": "wsl.exe",
      "args": [
        "bash",
        "-ic",
        "node /your/path/to/yaBMS/mcp/build/index.js"
      ]
    }
  }
}
```

### case of MCP server on Windows
```json
{
  "mcpServers": {
    "yabmswin": {
      "command": "node.exe",
      "args": [
        "C:\\path\\to\\yaBMS\\mcp\\build\\index.js"
      ]
    }
  }
}
```
Adjust the path as necessary for your environment.

## MCP Client usage
Ask your LLM like this:
```
Please expand (0,0,0)(1,1,1)(2,1,0)(1,1,1)[3] by yabms.
```
Then your LLM will anser like this:
```
The expansion of the YABMS expression (0,0,0)(1,1,1)(2,1,0)(1,1,1)[3] is:
(0,0,0)(1,1,1)(2,1,0)(1,1,0)(2,2,1)(3,2,0)(2,2,0)(3,3,1)(4,3,0)(3,3,0)(4,4,1)(5,4,0)[3]
YABMS (Yet Another Binary Multidimensional Sequence) is a notation system used to represent sequences with specific transformation rules.
```