.model small
.stack 256
.data
    file_info       db 30 dup(?)
    file_name       db 15 dup('$')
    output_buff     db 13, ?, 14 dup(?)
    file_mask       db 4, ?, 5 dup(0)
    ext_msg         db 'Enter extension: $'
    file_name_msg   db 13, 10, 'Enter output file name: $' 
	done_msg		db 13, 10, 'Done', 13, 10, '$'
.code


main:
    mov ax, @data
    mov ds, ax
    mov es, ax
    
    mov ah, 9h
    lea dx, ext_msg
    int 21h    
    mov ah, 0ah
    lea dx, file_mask
    int 21h
	add dl, file_mask + 1
	mov di, dx
	mov byte ptr [di + 2], 0
    mov file_mask, '*'
	mov file_mask + 1, '.'
	
    mov ah, 9h
    lea dx, file_name_msg
    int 21h    
    mov ah, 0ah
    lea dx, output_buff
    int 21h    
	add dl, output_buff + 1
	mov di, dx
	mov byte ptr [di + 2], 0
	
    mov ah, 3dh
	mov al, 1
    lea dx, output_buff + 2
    int 21h
    jc file_not_found
    mov bx, ax
    mov ah, 42h
    mov al, 2
    xor cx, cx
    xor dx, dx
    int 21h
    jmp file_opened
    
file_not_found:
	xor cx, cx
    mov ah, 3ch
    int 21h
    mov bx, ax

file_opened:
    mov ah, 1ah
    lea dx, file_info
    int 21h
    
    mov ah, 4eh
    mov cx, 2
    lea dx, file_mask
    int 21h
    jc done
    
    lea dx, file_name
    mov ah, 40h
	mov cx, 12
    int 21h
	
find_new:	
	mov ah, 4fh
    lea dx, file_mask
    int 21h
    jc done
    
    lea dx, file_name
    mov ah, 40h
	mov cx, 12
    int 21h
	jmp find_new
	
done:
	mov ah, 3eh
	int 21h
    
    mov ah, 9h
    lea dx, done_msg
    int 21h    
	
    mov ax, 4c00h
    int 21h
end main