#!/usr/bin/env node

import { McpServer } from "@modelcontextprotocol/sdk/server/mcp.js";
import { StdioServerTransport } from "@modelcontextprotocol/sdk/server/stdio.js";
import { z } from "zod";

const { Bms } = require("./bms.cjs");

// Create server instance
const server = new McpServer({
  name: "yabms",
  version: "1.0.0",
  capabilities: {
    resources: {},
    tools: {},
  },
});

// Register "expand_bms" tool
server.tool(
  "expand_bms",
  "expand bms",
  {
    str: z.string(),
  },
  async ({ str }) => {
    let result;
    try {
      result = Bms.str2expand(str);
    } catch (e) {
      result = `Error: ${(e as any).message}`;
    }
    return {
      content: [
        {
          type: "text",
          text: result,
        },
      ],
    };
  },
);

async function main() {
  const transport = new StdioServerTransport();
  await server.connect(transport);
  console.error("yaBMS MCP Server running on stdio");
}

main().catch((error) => {
  console.error("Fatal error in main():", error);
  process.exit(1);
});
