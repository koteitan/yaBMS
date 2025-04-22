#!/usr/bin/env node
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const mcp_js_1 = require("@modelcontextprotocol/sdk/server/mcp.js");
const stdio_js_1 = require("@modelcontextprotocol/sdk/server/stdio.js");
const zod_1 = require("zod");
const { Bms } = require("./bms.js");
// Create server instance
const server = new mcp_js_1.McpServer({
    name: "yabms",
    version: "1.0.0",
    capabilities: {
        resources: {},
        tools: {},
    },
});
// Register "expand_bms" tool
server.tool("expand_bms", "expand bms", {
    str: zod_1.z.string(),
}, async ({ str }) => {
    let result;
    try {
        result = Bms.str2expand(str);
    }
    catch (e) {
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
async function main() {
    const transport = new stdio_js_1.StdioServerTransport();
    await server.connect(transport);
    console.error("yaBMS MCP Server running on stdio");
}
main().catch((error) => {
    console.error("Fatal error in main():", error);
    process.exit(1);
});
