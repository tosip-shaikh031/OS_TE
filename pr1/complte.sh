#!/bin/bash

ADDRESS_BOOK="book.txt"

# ------------------ VALIDATION FUNCTIONS ------------------
validateName() {
    [[ "$1" =~ ^[a-zA-Z\s]+$ ]]
}

validatePhone() {
    [[ "$1" =~ ^[0-9]{10}$ ]]
}

validateEmail() {
    [[ "$1" =~ ^[a-zA-Z0-9._%+-]+@[a-zA-Z]+\.[a-zA-Z]{2,}$ ]]
}

# ------------------ CORE FUNCTIONS ------------------
create_address_book() {
    > "$ADDRESS_BOOK"
    echo "✅ Address Book Created (file: $ADDRESS_BOOK)"
}

view_address_book() {
    if [[ ! -f "$ADDRESS_BOOK" || ! -s "$ADDRESS_BOOK" ]]; then
        echo "📭 Address book is empty or doesn't exist."
        return
    fi

    echo "📖 Address Book Contents:"
    echo "-----------------------------"
    nl -w2 -s'. ' "$ADDRESS_BOOK" | while read -r num line; do
        IFS='|' read -r name phone email <<< "$line"
        echo "Record #$num"
        echo "Name : $name"
        echo "Phone: $phone"
        echo "Email: $email"
        echo "-----------------------------"
    done
}

insert_record() {
    echo "➕ Add a New Record"

    while true; do
        read -p "Enter Name: " name
        validateName "$name" && break || echo "❌ Invalid name (letters & spaces only)."
    done

    while true; do
        read -p "Enter Phone (10 digits): " phone
        validatePhone "$phone" && break || echo "❌ Invalid phone number (10 digits only)."
    done

    while true; do
        read -p "Enter Email: " email
        validateEmail "$email" && break || echo "❌ Invalid email format."
    done

    echo "$name|$phone|$email" >> "$ADDRESS_BOOK"
    echo "✅ Record added successfully."
}

delete_record() {
    if [[ ! -f "$ADDRESS_BOOK" || ! -s "$ADDRESS_BOOK" ]]; then
        echo "📭 Address book is empty or doesn't exist."
        return
    fi

    view_address_book
    read -p "Enter name to delete: " name

    match_count=$(grep -i -c "^$name|" "$ADDRESS_BOOK")
    if [[ $match_count -eq 0 ]]; then
        echo "❌ No record found with the name '$name'."
        return
    fi

    read -p "Are you sure you want to delete all records named '$name'? [y/N]: " confirm
    if [[ "$confirm" =~ ^[Yy]$ ]]; then
        sed -i "/^$name|/Id" "$ADDRESS_BOOK"
        echo "🗑️ Deleted $match_count record(s) with the name '$name'."
    else
        echo "❎ Deletion cancelled."
    fi
}

modify_record() {
    if [[ ! -f "$ADDRESS_BOOK" || ! -s "$ADDRESS_BOOK" ]]; then
        echo "📭 Address book is empty or doesn't exist."
        return
    fi

    view_address_book
    read -p "Enter record number to modify: " rec_num

    if ! [[ "$rec_num" =~ ^[0-9]+$ ]]; then
        echo "❌ Invalid input. Enter a numeric value."
        return
    fi

    total_lines=$(wc -l < "$ADDRESS_BOOK")
    (( rec_num < 1 || rec_num > total_lines )) && echo "❌ Record number out of range." && return

    current_record=$(sed -n "${rec_num}p" "$ADDRESS_BOOK")
    IFS='|' read -r old_name old_phone old_email <<< "$current_record"

    echo "Editing Record #$rec_num:"
    echo "Old Name : $old_name"
    echo "Old Phone: $old_phone"
    echo "Old Email: $old_email"

    read -p "Enter new name (leave blank to keep unchanged): " new_name
    read -p "Enter new phone (leave blank to keep unchanged): " new_phone
    read -p "Enter new email (leave blank to keep unchanged): " new_email

    [[ -z "$new_name" ]] && new_name="$old_name"
    [[ -z "$new_phone" ]] && new_phone="$old_phone"
    [[ -z "$new_email" ]] && new_email="$old_email"

    if ! validateName "$new_name"; then echo "❌ Invalid name."; return; fi
    if ! validatePhone "$new_phone"; then echo "❌ Invalid phone."; return; fi
    if ! validateEmail "$new_email"; then echo "❌ Invalid email."; return; fi

    sed -i "${rec_num}s/.*/$new_name|$new_phone|$new_email/" "$ADDRESS_BOOK"
    echo "✅ Record #$rec_num updated successfully."
}

search_by_rec_num() {
    if [[ ! -f "$ADDRESS_BOOK" || ! -s "$ADDRESS_BOOK" ]]; then
        echo "📭 Address book is empty or doesn't exist."
        return
    fi

    read -p "Enter record number to search: " rec_num
    if ! [[ "$rec_num" =~ ^[0-9]+$ ]]; then
        echo "❌ Invalid input. Please enter a number."
        return
    fi

    total_lines=$(wc -l < "$ADDRESS_BOOK")
    (( rec_num < 1 || rec_num > total_lines )) && echo "❌ Record number out of range (1–$total_lines)." && return

    record=$(sed -n "${rec_num}p" "$ADDRESS_BOOK")
    IFS='|' read -r name phone email <<< "$record"
    echo "-----------------------------"
    echo "Record #$rec_num"
    echo "Name : $name"
    echo "Phone: $phone"
    echo "Email: $email"
    echo "-----------------------------"
}

search_by_name() {
    if [[ ! -f "$ADDRESS_BOOK" || ! -s "$ADDRESS_BOOK" ]]; then
        echo "📭 Address book is empty or doesn't exist."
        return
    fi

    read -p "Enter name to search: " name
    if ! validateName "$name"; then
        echo "❌ Invalid name format."
        return
    fi

    matches=$(grep -in "^$name|" "$ADDRESS_BOOK")
    [[ -z "$matches" ]] && echo "❌ No records found with name '$name'." && return

    echo "🔍 Records matching '$name':"
    echo "-----------------------------"
    echo "$matches" | while IFS=: read -r line_num record; do
        IFS='|' read -r sname sphone semail <<< "$record"
        echo "Record #$line_num"
        echo "Name : $sname"
        echo "Phone: $sphone"
        echo "Email: $semail"
        echo "-----------------------------"
    done
}

sort_by_name() {
    if [[ ! -f "$ADDRESS_BOOK" || ! -s "$ADDRESS_BOOK" ]]; then
        echo "📭 Address book is empty or doesn't exist."
        return
    fi

    sort -t '|' -k1,1 -f "$ADDRESS_BOOK" -o "$ADDRESS_BOOK"
    echo "✅ Address book sorted by name successfully."
}

# ------------------ MAIN MENU ------------------
while true; do
    echo ""
    echo "========== 📒 ADDRESS BOOK MENU =========="
    echo "1) Create Address Book"
    echo "2) View Address Book"
    echo "3) Insert a Record"
    echo "4) Delete a Record"
    echo "5) Modify a Record"
    echo "6) Search by Record Number"
    echo "7) Search by Name"
    echo "8) Sort Records by Name"
    echo "9) Exit"
    echo "=========================================="
    read -p "Enter your choice (1-9): " choice
    echo ""

    case "$choice" in
        1) create_address_book ;;
        2) view_address_book ;;
        3) insert_record ;;
        4) delete_record ;;
        5) modify_record ;;
        6) search_by_rec_num ;;
        7) search_by_name ;;
        8) sort_by_name ;;
        9) echo "👋 Exiting Address Book. Goodbye!"; break ;;
        *) echo "❌ Invalid choice. Please select between 1-9." ;;
    esac
done
