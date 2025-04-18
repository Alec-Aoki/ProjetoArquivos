#!/usr/bin/env python3

import struct
import sys

# Define constants from the C code
TAM_DESC_ID = 23
TAM_DESC_YEAR = 27
TAM_DESC_FIN_LOSS = 28
TAM_DESC_COUNTRY = 26
TAM_DESC_TYPE = 38
TAM_DESC_TGT_IND = 38
TAM_DESC_DEF = 67

class Header:
    def __init__(self):
        self.status = ''
        self.topo = 0
        self.proxByteOffset = 0
        self.nroRegArq = 0
        self.nroRegRem = 0
        self.descreveIdentificador = ''
        self.descreveYear = ''
        self.descreveFinancialLoss = ''
        self.codDescreveCountry = ''
        self.descreveCountry = ''
        self.codDescreveType = ''
        self.descreveType = ''
        self.codDescreveTargetIndustry = ''
        self.descreveTargetIndustry = ''
        self.codDescreveDefense = ''
        self.descreveDefense = ''

class Record:
    def __init__(self):
        self.removido = ''
        self.tamanhoRegistro = 0
        self.prox = 0
        self.idAttack = 0
        self.year = 0
        self.financialLoss = 0.0
        self.country = ''
        self.attackType = ''
        self.targetIndustry = ''
        self.defenseMechanism = ''

def read_header(file):
    """Read the header from the binary file"""
    header = Header()
    
    # Read the variable fields
    header.status = file.read(1).decode('ascii')
    header.topo = struct.unpack('q', file.read(8))[0]  # long int is 8 bytes
    header.proxByteOffset = struct.unpack('q', file.read(8))[0]
    header.nroRegArq = struct.unpack('i', file.read(4))[0]  # int is 4 bytes
    header.nroRegRem = struct.unpack('i', file.read(4))[0]
    
    # Read the semantic fields
    header.descreveIdentificador = file.read(TAM_DESC_ID).decode('ascii')
    header.descreveYear = file.read(TAM_DESC_YEAR).decode('ascii')
    header.descreveFinancialLoss = file.read(TAM_DESC_FIN_LOSS).decode('ascii')
    
    header.codDescreveCountry = file.read(1).decode('ascii')
    header.descreveCountry = file.read(TAM_DESC_COUNTRY).decode('ascii')
    
    header.codDescreveType = file.read(1).decode('ascii')
    header.descreveType = file.read(TAM_DESC_TYPE).decode('ascii')
    
    header.codDescreveTargetIndustry = file.read(1).decode('ascii')
    header.descreveTargetIndustry = file.read(TAM_DESC_TGT_IND).decode('ascii')
    
    header.codDescreveDefense = file.read(1).decode('ascii')
    header.descreveDefense = file.read(TAM_DESC_DEF).decode('ascii')
    
    return header

def read_record(file):
    """Read a record from the binary file"""
    record = Record()
    
    # Read fixed-size fields
    record.removido = file.read(1).decode('ascii')
    record.tamanhoRegistro = struct.unpack('i', file.read(4))[0]
    record.prox = struct.unpack('q', file.read(8))[0]
    record.idAttack = struct.unpack('i', file.read(4))[0]
    record.year = struct.unpack('i', file.read(4))[0]
    record.financialLoss = struct.unpack('f', file.read(4))[0]
    
    # Read variable-length fields with delimiters
    content = file.read(record.tamanhoRegistro - 25)  # 25 is the size of the fixed fields
    
    # Parse the variable length fields which are in format like "1Brazil|2Ransomware|3Finance|4Patch Management|"
    content_str = content.decode('ascii')
    
    # Extract variable fields by finding delimiters
    fields = {}
    current_field = ""
    current_id = ""
    
    i = 0
    while i < len(content_str):
        if content_str[i] in "1234":  # Field identifiers
            current_id = content_str[i]
            i += 1
            current_field = ""
            while i < len(content_str) and content_str[i] != '|':
                current_field += content_str[i]
                i += 1
            fields[current_id] = current_field
        else:
            i += 1  # Skip delimiters or unexpected characters
    
    # Assign fields based on their identifiers
    record.country = fields.get('1', '')
    record.attackType = fields.get('2', '')
    record.targetIndustry = fields.get('3', '')
    record.defenseMechanism = fields.get('4', '')
    
    return record

def convert_to_text(bin_filename, txt_filename):
    """Convert binary file to text file"""
    try:
        with open(bin_filename, 'rb') as bin_file, open(txt_filename, 'w', encoding='utf-8') as txt_file:
            # Read header
            header = read_header(bin_file)
            
            # Write header information to text file
            txt_file.write("===== HEADER INFORMATION =====\n")
            txt_file.write(f"Status: {header.status}\n")
            txt_file.write(f"Top: {header.topo}\n")
            txt_file.write(f"Next Byte Offset: {header.proxByteOffset}\n")
            txt_file.write(f"Number of Records: {header.nroRegArq}\n")
            txt_file.write(f"Number of Removed Records: {header.nroRegRem}\n")
            txt_file.write(f"Description - ID: {header.descreveIdentificador}\n")
            txt_file.write(f"Description - Year: {header.descreveYear}\n")
            txt_file.write(f"Description - Financial Loss: {header.descreveFinancialLoss}\n")
            txt_file.write(f"Description - Country: {header.descreveCountry}\n")
            txt_file.write(f"Description - Attack Type: {header.descreveType}\n")
            txt_file.write(f"Description - Target Industry: {header.descreveTargetIndustry}\n")
            txt_file.write(f"Description - Defense Mechanism: {header.descreveDefense}\n")
            txt_file.write("\n===== RECORDS =====\n")
            
            # Read and write records
            record_count = 0
            current_offset = 276  # Header size
            
            while current_offset < header.proxByteOffset:
                bin_file.seek(current_offset)
                try:
                    record = read_record(bin_file)
                    record_count += 1
                    
                    txt_file.write(f"\nRecord #{record_count}:\n")
                    txt_file.write(f"Removed: {record.removido}\n")
                    txt_file.write(f"Record Size: {record.tamanhoRegistro} bytes\n")
                    txt_file.write(f"Next Removed: {record.prox}\n")
                    txt_file.write(f"ID Attack: {record.idAttack}\n")
                    txt_file.write(f"Year: {record.year}\n")
                    txt_file.write(f"Financial Loss: {record.financialLoss:.2f}\n")
                    txt_file.write(f"Country: {record.country}\n")
                    txt_file.write(f"Attack Type: {record.attackType}\n")
                    txt_file.write(f"Target Industry: {record.targetIndustry}\n")
                    txt_file.write(f"Defense Mechanism: {record.defenseMechanism}\n")
                    
                    # Move to the next record
                    current_offset += record.tamanhoRegistro
                except Exception as e:
                    print(f"Error reading record at offset {current_offset}: {e}")
                    break
            
            # After processing all records, add the byte offset information
            txt_file.write("\n===== BYTE OFFSET INFORMATION =====\n")
            txt_file.write(f"Expected next free byteOffset (calculated): {current_offset}\n")
            txt_file.write(f"Next free byteOffset from header: {header.proxByteOffset}\n")
            
            print(f"Successfully converted {record_count} records to {txt_filename}")
    
    except Exception as e:
        print(f"Error: {e}")
        return False
    
    return True

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python binary_reader.py <binary_file> <output_text_file>")
        sys.exit(1)
    
    bin_filename = sys.argv[1]
    txt_filename = sys.argv[2]
    
    if convert_to_text(bin_filename, txt_filename):
        print("Conversion completed successfully.")
    else:
        print("Conversion failed.")