#!/usr/bin/env node
"use strict";

const { Server } = require("@modelcontextprotocol/sdk/server/index.js");
const { StdioServerTransport } = require("@modelcontextprotocol/sdk/server/stdio.js");
const {
  CallToolRequestSchema,
  ListResourcesRequestSchema,
  ListToolsRequestSchema,
  ReadResourceRequestSchema,
} = require("@modelcontextprotocol/sdk/types.js");
const { z } = require("zod");

const { Bms } = require("./bms.js");

// Define input schema for expand_bms tool
const ExpandBmsInputSchema = {
  type: "object",
  properties: {
    str: {
      type: "string",
      description: "BMS string to expand",
      required: true,
    },
  },
};

// Create server instance
const server = new Server(
  {
    name: "yabms",
    version: "1.0.0",
  },
  {
    capabilities: {
      tools: {
        expand_bms: {
          description: "Expand BMS string",
          inputSchema: ExpandBmsInputSchema,
        },
      },
      resources: {},
    },
  }
);

// Handle ListToolsRequestSchema
server.setRequestHandler(ListToolsRequestSchema, async () => {
  return {
    tools: [
      {
        name: "expand_bms",
        description: "Expand BMS string",
        inputSchema: ExpandBmsInputSchema,
      },
    ],
  };
});

// Handle ListResourcesRequestSchema
server.setRequestHandler(ListResourcesRequestSchema, async () => {
  return {
    resources: [],
  };
});

// Handle CallToolRequestSchema
server.setRequestHandler(CallToolRequestSchema, async (request) => {
  if (request.params.name !== "expand_bms") {
    throw new Error("Unknown tool: " + request.params.name);
  }
  const str = request.params.arguments.str;
  let result;
  try {
    result = Bms.str2expand(str);
  } catch (e) {
    result = `Error: ${e.message}`;
  }
  return {
    content: [
      {
        type: "text",
        text: result,
      },
    ],
  };
});

// Handle ReadResourceRequestSchema
server.setRequestHandler(ReadResourceRequestSchema, async () => {
  return {
    content: [],
  };
});

async function main() {
  const transport = new StdioServerTransport();
  await server.connect(transport);
  console.error("yaBMS MCP Server running on stdio");
}

main().catch((error) => {
  console.error("Fatal error in main():", error);
  process.exit(1);
});
