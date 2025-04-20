// MCP server for yaBMS
const { McpServer } = require('@modelcontextprotocol/sdk/server/mcp.js');
const { StdioServerTransport } = require('@modelcontextprotocol/sdk/server/stdio.js');
const path = require('path');
const fs = require('fs');

// Import yaBMS JavaScript files
const jsPath = path.join(__dirname, '../js');

// Custom require function to load browser JS files into Node.js
function requireBrowserJS(filePath) {
  const content = fs.readFileSync(filePath, 'utf8');
  const wrappedContent = `
    (function(exports) {
      ${content}
      return { exports };
    })({});
  `;
  // Execute the wrapped content in a new context
  const result = eval(wrappedContent);
  return result.exports;
}

// Load yaBMS JavaScript files
try {
  console.log('Loading yaBMS JavaScript modules...');
  
  // Load dependencies first
  global.exarray = requireBrowserJS(path.join(jsPath, 'exarray.js')).exarray;
  global.matrix = requireBrowserJS(path.join(jsPath, 'matrix.js')).matrix;
  
  // Then load the main bms.js file
  global.bms = requireBrowserJS(path.join(jsPath, 'bms.js')).bms;
  
  console.log('yaBMS modules loaded successfully');
} catch (error) {
  console.error('Error loading yaBMS modules:', error);
  process.exit(1);
}

// Create MCP server
const server = new McpServer({ 
  name: "yaBMS Server", 
  version: "1.0.0" 
});

// Register expand-matrix tool
server.tool("expand-matrix", {
  matrix: { 
    type: 'string', 
    description: 'Bashicu Matrix in string format, e.g., "(0,0,0)(1,1,1)(2,1,0)(1,1,1)[2]"' 
  },
  async ({ matrix }) => {
    try {
      // Input validation
      if (!matrix || typeof matrix !== 'string') {
        throw new Error('Matrix parameter must be a non-empty string');
      }

      console.log(`Expanding matrix: ${matrix}`);
      
      // Call the str2expand function
      let result;
      try {
        // Call bms.str2expand with just the matrix string
        result = bms.str2expand(matrix);
      } catch (error) {
        console.error('Error during BMS expansion:', error);
        throw new Error(`Failed to expand matrix: ${error.message}`);
      }
      
      return { 
        content: [{ 
          type: "text", 
          text: result 
        }]
      };
    } catch (error) {
      console.error('Error processing request:', error);
      throw new Error(`Failed to process request: ${error.message}`);
    }
  }
});

// Initialize transport (using StdioServerTransport by default)
const transport = new StdioServerTransport();

// Start server
server.connect(transport).then(() => {
  console.log(`MCP yaBMS server started using stdio transport`);
  console.log('Available tools:');
  console.log('- expand-matrix');
}).catch((err) => {
  console.error('Failed to start MCP server:', err);
  process.exit(1);
});

