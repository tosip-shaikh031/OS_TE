#!/bin/bash
FILE="addressbook.txt"
if [ ! -f "$FILE" ]; then
    touch "$FILE"
fi
add_entry() {
    echo "Enter ID:"
    read id
    if grep -q "^$id:" "$FILE"; then
        echo "ID already exists!"
        return
    fi
    echo "Enter Name:"
    read name
    echo "Enter Phone:"
    read phone
    echo "Enter Email:"
    read email
    echo "$id:$name:$phone:$email" >> "$FILE"
    echo "Entry added successfully!"
}
search_entry() {
    echo "Enter keyword (ID/Name/Phone/Email) to search:"
    read key
    result=$(grep -i "$key" "$FILE")
    if [ -z "$result" ]; then
        echo "No matching records found."
    else
        echo "Matching Records:"
        echo "$result" | column -t -s":"
    fi
}
display_all() {
    if [ ! -s "$FILE" ]; then
        echo "Address book is empty!"
    else
        echo "All Records:"
        echo "-------------------------------------------"
        column -t -s":" "$FILE"
        echo "-------------------------------------------"
    fi
}
remove_entry() {
    echo "Enter ID to delete:"
    read id
    if grep -q "^$id:" "$FILE"; then
        grep -v "^$id:" "$FILE" > temp.txt && mv temp.txt "$FILE"
        echo "Record deleted successfully!"
    else
        echo "Record not found."
    fi
}
edit_entry() {
    echo "Enter ID to edit:"
    read id
    if ! grep -q "^$id:" "$FILE"; then
        echo "No record found with ID $id."
        return
    fi
    old_record=$(grep "^$id:" "$FILE")
    IFS=':' read -r old_id old_name old_phone old_email <<< "$old_record"
    echo "Editing record: $old_name ($old_phone, $old_email)"
    echo "Leave blank to keep the existing value."
    echo -n "New Name [$old_name]: "
    read new_name
    echo -n "New Phone [$old_phone]: "
    read new_phone
    echo -n "New Email [$old_email]: "
    read new_email
    new_name=${new_name:-$old_name}
    new_phone=${new_phone:-$old_phone}
    new_email=${new_email:-$old_email}
    grep -v "^$id:" "$FILE" > temp.txt
    echo "$id:$new_name:$new_phone:$new_email" >> temp.txt
    mv temp.txt "$FILE"

    echo "Record updated successfully!"
}
while true; do
    echo ""
    echo "========= ADDRESS BOOK MENU ========="
    echo "1. Search Address Book"
    echo "2. Add Entry"
    echo "3. Remove Entry"
    echo "4. Edit Entry"
    echo "5. Display All Records"
    echo "6. Quit"
    echo "====================================="
    echo -n "Enter your choice [1-6]: "
    read choice
    case $choice in
        1) search_entry ;;
        2) add_entry ;;
        3) remove_entry ;;
        4) edit_entry ;;
        5) display_all ;;
        6) echo "Exiting program. Bye!"; exit 0 ;;
        *) echo "Invalid choice! Please enter between 1–6." ;;
    esac
done