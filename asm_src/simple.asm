 .org $8000

reset:
    lda #$ff
    sta $6002

loop:
    ; one 
    lda #%00000110
    sta $6000

    ; clear
    lda #%00000000
    sta $6000

    ; three
    lda #%00011111
    sta $6000

    ; clear
    lda #%00000000
    sta $6000

    ; three
    lda #%00011111
    sta $6000

    ; clear
    lda #%00000000
    sta $6000

    ; seven
    lda #%00000111
    sta $6000

    ; clear
    lda #%00000000
    sta $6000
    
    jmp loop