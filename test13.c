//gr3ypkg, gr3yos "package manager"
//this version removes bin/sh
CMD="$1"
NAME="$2"
SRC="$3"

PKG_DIR="$HOME/.gr3ypkg"

//this makes package directory if it doesn't exist
mkdir -p "$PKG_DIR"

case "$CMD" in
    get)
        if [ -z "$NAME" ] || [ -z "$SRC" ]; then
            echo "Usage: gr3ypkg get [name] [url]"
            exit 1
        fi

        echo "[GR3YPKG] Downloading $NAME..."

        //checks if the source is a compressed package pack
        case "$SRC" in
            *.tar.gz)
                //makes a dedicated directory for this pack to avoid clutter
                TARGET_DIR="$PKG_DIR/$NAME"
                mkdir -p "$TARGET_DIR"
                
                //downloads and pipes directly to tar for extraction
                curl -L "$SRC" | tar -xzC "$TARGET_DIR"
                
                //makes everything inside executable (or target specific binaries)
                chmod -R +x "$TARGET_DIR"
                echo "[GR3YPKG] Extracted package pack to $TARGET_DIR"
                ;;
            *)
                //installation
                FILE="$PKG_DIR/$NAME"
                curl -L "$SRC" -o "$FILE"
                chmod +x "$FILE"
                echo "[GR3YPKG] Installed $NAME to $FILE"
                ;;
        esac
        ;;

    run)
        if [ -z "$NAME" ]; then
            echo "Usage: gr3ypkg run [name]"
            exit 1
        fi

        FILE="$PKG_DIR/$NAME"

        // if it's a package pack directory, look for a binary matching the folder name inside it
        if [ -d "$FILE" ]; then
            if [ -f "$FILE/$NAME" ]; then
                "$FILE/$NAME"
            else
                echo "Pack directory found, but launcher binary '$FILE/$NAME' not found."
                exit 1
            fi
        elif [ -f "$FILE" ]; then
            "$FILE"
        else
            echo "Package not found: $NAME"
            exit 1
        fi
        ;;

    list)
        echo "Installed packages and packs:"
        ls "$PKG_DIR"
        ;;

    remove)
        if [ -z "$NAME" ]; then
            echo "Usage: gr3ypkg remove [name]"
            exit 1
        fi

        //removes either the single binary file or the package pack folder
        rm -rf "$PKG_DIR/$NAME"
        echo "Removed $NAME"
        ;;
        
    *)
        //commands, even though it's pretty simple, may be useful.
        echo "gr3ypkg commands:"
        echo "  get [name] [url]"
        echo "  run [name]"
        echo "  list"
        echo "  remove [name]"
        ;;
esac