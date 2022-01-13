|      | Stack           | in         | out   |      | Memory            | in        | out |
|------|-----------------|------------|-------|------|-------------------|-----------|-----|
| 0x00 | BRK/LIT Break   |            | num   | 0x10 | LDZ LoadZeropage  | addr^     | val |
| 0x01 | INC Increment   | a          | a+1   | 0x11 | STZ StoreZeropage | val addr^ |     |
| 0x02 | POP Pop         | a          |       | 0x12 | LDR LoadRelative  | addr^     | val |
| 0x03 | DUP Duplicate   | a          | a a   | 0x13 | STR StoreRelative | val addr^ |     |
| 0x04 | NIP Nip         | a b        | b     | 0x14 | LDA LoadAbsolute  | addr*     | val |
| 0x05 | SWP Swap        | a b        | b a   | 0x15 | STA StoreAbsolute | val addr* |     |
| 0x06 | OVR Over        | a b        | a b a | 0x16 | DEI Device In     | addr^     | val |
| 0x07 | ROT Rotate      | a b c      | b c a | 0x17 | DEO Device Out    | val addr^ |     |

|      | Logic           | in         | out   |      | Arithmetic        | in        | out |
|------|-----------------|------------|-------|------|-------------------|-----------|-----|
| 0x08 | EQU Equal       | a b        | bool^ | 0x18 | ADD Add           | a b       | a+b |
| 0x09 | NEQ NotEqual    | a b        | bool^ | 0x19 | SUB Subtract      | a b       | a-b |
| 0x0a | GTH GreaterThan | a b        | bool^ | 0x1a | MUL Multiply      | a b       | a*b |
| 0x0b | LTH LesserThan  | a b        | bool^ | 0x1b | DIV Divide        | a b       | a/b |
| 0x0c | JMP Jump        | addr       |       | 0x1c | AND And           | a b       | res |
| 0x0d | JCN JumpCond    | bool^ addr |       | 0x1d | ORA Or            | a b       | res |
| 0x0e | JSR JumpStash   | addr       |       | 0x1e | EOR ExclusiveOr   | a b       | res |
| 0x0f | STH Stash       | a          |       | 0x1f | SFT Shift         | a b^      | res |

