#!/bin/sh

# GR3YOS Package Tool - gr3ypkg

CMD="$1"
NAME="$2"
SRC="$3"

PKG_DIR="$HOME/.gr3ypkg"

# Create package directory if it doesn't exist
mkdir -p "$PKG_DIR"

case "$CMD" in
    get)
        if [ -z "$NAME" ] || [ -z "$SRC" ]; then
            echo "Usage: gr3ypkg get [name] [url]"
            exit 1
        fi

        echo "[GR3YPKG] Downloading $NAME..."

        FILE="$PKG_DIR/$NAME"

        # Try curl, fallback to wget
        if command -v curl >/dev/null 2>&1; then
            curl -L "$SRC" -o "$FILE"
        elif command -v wget >/dev/null 2>&1; then
            wget "$SRC" -O "$FILE"
        else
            echo "Error: curl or wget required"
            exit 1
        fi

        chmod +x "$FILE"

        echo "[GR3YPKG] Installed $NAME to $FILE"
        ;;

    run)
        if [ -z "$NAME" ]; then
            echo "Usage: gr3ypkg run [name]"
            exit 1
        fi

        FILE="$PKG_DIR/$NAME"

        if [ ! -f "$FILE" ]; then
            echo "Package not found: $NAME"
            exit 1
        fi

        "$FILE"
        ;;

    list)
        echo "Installed packages:"
        ls "$PKG_DIR"
        ;;

    remove)
        if [ -z "$NAME" ]; then
            echo "Usage: gr3ypkg remove [name]"
            exit 1
        fi

        rm -f "$PKG_DIR/$NAME"
        echo "Removed $NAME"
        ;;

    *)
        echo "gr3ypkg commands:"
        echo "  get [name] [url]"
        echo "  run [name]"
        echo "  list"
        echo "  remove [name]"
        ;;
esac
