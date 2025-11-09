#!/bin/bash
# Address Book Script
FILE="address.txt"
if [ ! -f "$FILE" ]; then
    touch "$FILE"
fi
add_entry() {
    echo "Enter ID:"
    read id
    if grep -q "^$id;" "$FILE"; then 
        echo "Record with ID $id already exists!"
        return
    fi
    echo "Enter Name:"
    read name
    echo "Enter Phone:"
    read phone
    echo "$id;$name;$phone" >> "$FILE"
    echo "Record added successfully!"
}
search_entry() {
    echo "Enter ID or Name or Phone to search:"
    read keyword
    result=$(grep -i "$keyword" "$FILE")
    if [ -z "$result" ]; then
        echo "No matching record found."
    else
        echo "Matching Record(s):"
        echo "$result" | column -t -s";"
    fi
}
remove_entry() {
    echo "Enter ID to delete:"
    read id
    if grep -q "^$id;" "$FILE"; then
        grep -v "^$id;" "$FILE" > temp.txt && mv temp.txt "$FILE"
        echo "Record with ID $id removed."
    else
        echo "Record not found."
    fi
}
while true; do
    echo ""
    echo "========= ADDRESS BOOK ========="
    echo "1. Search Address Book"
    echo "2. Add an Address Book Entry"
    echo "3. Remove an Address Book Entry"
    echo "4. Quit"
    echo "================================"
    echo -n "Enter your choice [1-4]: "
    read choice
    case $choice in
        1) search_entry ;;
        2) add_entry ;;
        3) remove_entry ;;
        4) echo "Exiting Address Book. Bye!"; exit 0 ;;
        *) echo "Invalid choice! Please enter 1-4." ;;
    esac
done