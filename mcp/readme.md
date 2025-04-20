# MCP Server Setup and Usage

This document provides instructions for setting up and using the MCP server in this project.

## Prerequisites

Make sure you have the following installed on your system:

- WSL
- Node.js (version 20.x recommended)
- npm (Node.js package manager)
- TypeScript compiler (`tsc`)
- Required system packages

## Installation

```bash
cd mcp
npm install
npm run build
```

## cline MCP Settings

Add or update the following entries in your setting.json of the clients to configure the MCP servers:
- For **claude**, the configuration file is located at:
  `C:\Users\your-user-name\AppData\Roaming\Claude\claude_desktop_config.json`
- For **cline** by WSL, the configuration file is located at:
  `/home/your-user-name/.vscode-server/data/User/globalStorage/saoudrizwan.claude-dev/settings/cline_mcp_settings.json`

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

Adjust the path as necessary for your environment.

## Notes

- Ensure the MCP server is running before attempting to use MCP tools.
- If you modify the TypeScript source, remember to rebuild before restarting the server.
- For any issues, check the server logs for error messages.


