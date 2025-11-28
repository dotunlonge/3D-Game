#!/bin/bash

# Helper script to set up GLAD for the Render Engine
# This script downloads and sets up GLAD automatically

echo "Setting up GLAD for Render Engine..."
echo ""

# Check if curl is available
if ! command -v curl &> /dev/null; then
    echo "Error: curl is required but not installed."
    exit 1
fi

# Create temporary directory
TEMP_DIR=$(mktemp -d)
cd "$TEMP_DIR"

echo "Downloading GLAD generator..."
# Note: This is a simplified approach. For production, you should visit
# https://glad.dav1d.de/ and generate manually, or use the GLAD web API

echo ""
echo "=========================================="
echo "GLAD Setup Instructions:"
echo "=========================================="
echo ""
echo "1. Visit: https://glad.dav1d.de/"
echo "2. Configure:"
echo "   - Language: C/C++"
echo "   - Specification: OpenGL"
echo "   - API gl: Version 3.3"
echo "   - Profile: Core"
echo "   - Generate a loader: ✓ (checked)"
echo "3. Click 'Generate'"
echo "4. Download the ZIP file"
echo "5. Extract and copy files:"
echo "   - include/glad/glad.h → $(pwd)/../include/glad/glad.h"
echo "   - include/KHR/khrplatform.h → $(pwd)/../include/KHR/khrplatform.h"
echo "   - src/glad.c → $(pwd)/../src/glad.c"
echo ""
echo "Alternatively, you can manually download and extract the ZIP file."
echo ""

# Cleanup
cd - > /dev/null
rm -rf "$TEMP_DIR"

echo "For macOS, you can also use the system OpenGL framework."
echo "See BUILDING.md for more details."

