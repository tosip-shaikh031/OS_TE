#!/bin/bash
test_file() {
    echo "Enter filename to check:"
    read fname
    if [ -f "$fname" ]; then
        echo "File '$fname' exists."
    else
        echo "File '$fname' does not exist."
    fi
}
read_file() {
    echo "Enter filename to read:"
    read fname
    if [ -f "$fname" ]; then
        echo "Contents of '$fname':"
        echo "----------------------------------"
        cat "$fname"
        echo "----------------------------------"
    else
        echo "File '$fname' not found."
    fi
}
delete_file() {
    echo "Enter filename to delete:"
    read fname
    if [ -f "$fname" ]; then
        rm -i "$fname"   # -i asks confirmation before deleting
        echo "File deleted (if confirmed)."
    else
        echo "File '$fname' not found."
    fi
}
list_files() {
    echo "Files in current directory:"
    echo "----------------------------------"
    ls -lh --color=auto | awk '{print $9}' | tail -n +2
    echo "----------------------------------"
}
while true; do
    echo ""
    echo "========= FILE MANAGEMENT MENU ========="
    echo "1. Test if File Exists"
    echo "2. Read a File"
    echo "3. Delete a File"
    echo "4. Display List of Files"
    echo "5. Quit"
    echo "========================================"
    echo -n "Enter your choice [1-5]: "
    read choice

    case $choice in
        1) test_file ;;
        2) read_file ;;
        3) delete_file ;;
        4) list_files ;;
        5) echo "Exiting File Manager. Bye!"; exit 0 ;;
        *) echo "Invalid choice! Please enter 1–5." ;;
    esac
done