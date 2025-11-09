#!/bin/bash

ADDRESS_BOOK="book.txt"

function validateName(){
    [[ "$1" =~ ^[a-zA-Z\s]+$ ]] 
}

function validatePhone(){
    local digits_count
    digits_count=$(echo "$1" | grep -o '[0-9]' | wc -l)
    [[ "$1" =~ ^[0-9]+$ ]] && [ "$digits_count" -eq 10 ]
    # [[ "$1" =~ ^[0-9]{10}$ ]]
}

function validateEmail(){
    [[ "$1" =~ ^[a-zA-Z0-9]+@[a-zA-Z]+\.[a-zA-Z]{2,}$ ]]
}

function create_address_book(){
    > "$ADDRESS_BOOK" 
    echo "Address Book Created."
}
# function create_address_book(){
#     if [ ! -f "$ADDRESS_BOOK" ]; then
#         touch "$ADDRESS_BOOK"
#         echo "Address Book Created."
#     else
#         echo "Address Book already exists. No changes made."
#     fi
# }

function view_address_book(){
    if [ ! -f "$ADDRESS_BOOK" ] || [ ! -s "$ADDRESS_BOOK" ]; then
    	echo "Address book is empty or doesn't exist."
    	return
    fi
    cat "$ADDRESS_BOOK" | nl -w3 -s'. ' | while read -r num line; do
    	IFS='|' read -r name phone email <<< "$line"
    	echo "$num) Name: $name, Phone: $phone, Email: $email"
    done
    echo "------------------------"
}

function insert_record() {

    while true; do
        echo -n "Enter Name: "
    	read name
    	if validateName "$name"; then
    	    break
        else
    	    echo "Invalid name"
        fi
    done
    
    while true; do
        echo -n "Enter Phone Number: "
    	read phone
    	if validatePhone "$phone"; then
    	    break
        else
    	    echo "Invalid phone number"
        fi
    done
    while true; do
        echo -n "Enter Email: "
    	read email
    	if validateEmail "$email"; then
    	    break
        else
    	    echo "Invalid email"
        fi
    done
    echo "$name|$phone|$email" >> "$ADDRESS_BOOK"
    echo "Record added."
}

function delete_record() {
    if [ ! -f "$ADDRESS_BOOK" ] || [ ! -s "$ADDRESS_BOOK" ]; then
        echo "Address book is empty or doesn't exist."
        return
    fi

    view_address_book
    echo -n "Enter the name to delete: "
    read name

    # Check if name exists
    match_count=$(grep -i -c "^$name|" "$ADDRESS_BOOK")
    if [ "$match_count" -eq 0 ]; then
        echo "No record found with name: $name"
        return
    fi

    echo "Found $match_count record(s) matching \"$name\"."
    echo -n "Are you sure you want to delete all of them? [y/N]: "
    read confirm
    
    if [[ "$confirm" =~ ^[Yy]$ ]]; then
        # Delete all lines where name matches (case-insensitive)
        sed -i "/^$name|/Id" "$ADDRESS_BOOK" 
        echo "Deleted $match_count record(s) with name: $name"
    else
        echo "Deletion cancelled."
    fi
}

function modify_record() {
    if [ ! -f "$ADDRESS_BOOK" ] || [ ! -s "$ADDRESS_BOOK" ]; then
        echo "Address book is empty or doesn't exist."
        return
    fi

    view_address_book
    echo -n "Enter record number to modify: "
    read rec_num

    # Validate input: must be a number
    if ! [[ "$rec_num" =~ ^[0-9]+$ ]]; then
        echo "Invalid input. Please enter a number."
        return
    fi

    total_lines=$(wc -l < "$ADDRESS_BOOK")
    if [ "$rec_num" -lt 1 ] || [ "$rec_num" -gt "$total_lines" ]; then
        echo "Record number out of range."
        return
    fi

    current_record=$(sed -n "${rec_num}p" "$ADDRESS_BOOK")
    IFS='|' read -r old_name old_phone old_email <<< "$current_record"

    echo "Current Name : $old_name"
    echo "Current Phone: $old_phone"
    echo "Current Email: $old_email"

    echo -n "Enter new name (press Enter to keep unchanged): "
    read new_name
    [ -z "$new_name" ] && new_name="$old_name"

    echo -n "Enter new phone (press Enter to keep unchanged): "
    read new_phone
    [ -z "$new_phone" ] && new_phone="$old_phone"

    echo -n "Enter new email (press Enter to keep unchanged): "
    read new_email
    [ -z "$new_email" ] && new_email="$old_email"

    # Validate new inputs
    if ! validateName "$new_name"; then
        echo "Invalid name. Modification cancelled."
        return
    fi
    if ! validatePhone "$new_phone"; then
        echo "Invalid phone number. Modification cancelled."
        return
    fi
    if ! validateEmail "$new_email"; then
        echo "Invalid email. Modification cancelled."
        return
    fi

    new_record="$new_name|$new_phone|$new_email"

    sed -i "${rec_num}s/.*/$new_record/" "$ADDRESS_BOOK" 
    echo "Record updated."
}

# Main menu loop
while true; do
    echo "Address Book Menu:"
    echo "a) Create address book"
    echo "b) View address book"
    echo "c) Insert a record"
    echo "d) Delete a record"
    echo "e) Modify a record"
    echo "f) Exit"
    echo -n "Choose an option: "
    read option

    case "$option" in
        a|A) create_address_book ;;
        b|B) view_address_book ;;
        c|C) insert_record ;;
        d|D) delete_record ;;
        e|E) modify_record ;;
        f|F) echo "Exiting..."; break ;;
        *) echo "Invalid option. Try again." ;;
    esac
done


function search_by_rec_num() {
    if [ ! -f "$ADDRESS_BOOK" ] || [ ! -s "$ADDRESS_BOOK" ]; then
        echo "Address book is empty or doesn't exist."
        return
    fi

    echo -n "Enter record number to search: "
    read rec_num

    # Validate input (must be a number)
    if ! [[ "$rec_num" =~ ^[0-9]+$ ]]; then
        echo "Invalid input. Please enter a numeric record number."
        return
    fi

    total_lines=$(wc -l < "$ADDRESS_BOOK")

    if [ "$rec_num" -lt 1 ] || [ "$rec_num" -gt "$total_lines" ]; then
        echo "Record number out of range (1–$total_lines)."
        return
    fi

    # Fetch the specific record
    current_record=$(sed -n "${rec_num}p" "$ADDRESS_BOOK") 
    IFS='|' read -r name phone email <<< "$current_record"

    echo "-----------------------------"
    echo "Record #$rec_num"
    echo "Name : $name"
    echo "Phone: $phone"
    echo "Email: $email"
    echo "-----------------------------"
}

function search_by_name() {
    if [ ! -f "$ADDRESS_BOOK" ] || [ ! -s "$ADDRESS_BOOK" ]; then
        echo "Address book is empty or doesn't exist."
        return
    fi

    echo -n "Enter name to search: "
    read name

    # Validate name (only letters and spaces)
    if ! [[ "$name" =~ ^[a-zA-Z\s]+$ ]]; then
        echo "Invalid name format. Please use letters and spaces only."
        return
    fi

    # Search for name (case-insensitive)
    matches=$(grep -i "^$name|" "$ADDRESS_BOOK")

    if [ -z "$matches" ]; then
        echo "No records found with the name: $name"
        return
    fi

    echo "-----------------------------"
    echo "Records matching \"$name\":"
    echo

    # Display all matching records with line numbers
    grep -in "^$name|" "$ADDRESS_BOOK" | while IFS=: read -r line_num record; do
        IFS='|' read -r sname sphone semail <<< "$record"
        echo "Record #$line_num"
        echo "Name : $sname"
        echo "Phone: $sphone"
        echo "Email: $semail"
        echo "-----------------------------"
    done
}

function sortByName() {
    if [[ ! -f "$ADDRESS_BOOK" || ! -s "$ADDRESS_BOOK" ]]; then
        echo "Address book is empty or missing!"
        return
    fi

    # Sort by the first field (Name) alphabetically, case-insensitive
    sort -t ',' -k1,1 -f "$ADDRESS_BOOK" -o "$ADDRESS_BOOK"

    echo "Entries sorted by name successfully."
}
