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
const fs = require("fs");
const path = require("path");

function logSyslog(message) {
  const now = new Date();
  const timestamp = now.toISOString();
  const logMsg = `${timestamp} yabms: ${message}\n`;
  fs.mkdirSync(path.join(__dirname, "logs"), { recursive: true });
  fs.appendFileSync(path.join(__dirname, "logs", "syslog"), logMsg);
}

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
  logSyslog("Received ListToolsRequestSchema");
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

// Handle CallToolRequestSchema
server.setRequestHandler(CallToolRequestSchema, async (request) => {
  logSyslog("Received CallToolRequestSchema: " + JSON.stringify(request));
  let response;
  try {
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
    response = {
      content: [
        {
          type: "text",
          text: result,
        },
      ],
    };
  } catch (error) {
    response = { error: error.message };
    throw error;
  } finally {
    logSyslog("Returned CallToolRequestSchema: " + JSON.stringify(response));
  }
  return response;
});

// Handle ListResourcesRequestSchema
server.setRequestHandler(ListResourcesRequestSchema, async () => {
  logSyslog("Received ListResourcesRequestSchema");
  return {
    resources: [
      {
        uri: "bms://what_is_bms",
        name: "What is Bashicu Matrix System",
        description: "Explanation of Bashicu Matrix notation and expansion",
        mimeType: "text/markdown"
      }
    ]
  };
});

server.setRequestHandler(ReadResourceRequestSchema, async (request) => {
  let response;
  try {
    const uri = request.params.uri;
if (uri === "bms://what_is_bms") {
      const filePath = path.join(__dirname, "doc", "index.md");
      const text = fs.readFileSync(filePath, "utf8");
      response = {
        contents: [
          {
            uri: uri,
            mimeType: "text/markdown",
            text: text
          }
        ]
      };
    } else {
      throw new Error("Resource not found");
    }
  } catch (error) {
    response = { error: error.message };
    throw error;
  } finally {
    logSyslog(JSON.stringify({ type: "ReadResourceRequestSchema", request, response }));
  }
  return response;
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
