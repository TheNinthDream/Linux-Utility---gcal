#!/bin/bash

# Define the installation directory
INSTALL_DIR="$HOME/.local/bin"

# Run the make command to build the program
echo "Building the program..."
make

# Create the installation directory if it doesn't already exist
if [ ! -d "$INSTALL_DIR" ]; then
    echo "Creating installation directory: $INSTALL_DIR"
    mkdir -p "$INSTALL_DIR"
fi

# Copy the executable to the installation directory
echo "Copying executable to $INSTALL_DIR"
cp gcal "$INSTALL_DIR"

# Check if the directory is already in the PATH
if ! echo "$PATH" | grep -q "$INSTALL_DIR"; then
    echo "Adding $INSTALL_DIR to PATH in ~/.bashrc"
    echo "export PATH=\"$INSTALL_DIR:\$PATH\"" >> "$HOME/.bashrc"
    echo "Please run 'source ~/.bashrc' or restart your terminal to apply the changes."
else
    echo "$INSTALL_DIR is already in PATH"
fi

echo "Installation complete."
