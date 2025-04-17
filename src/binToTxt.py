# Made by Claude AI

#!/usr/bin/env python3
import struct
import sys

# Constants from registros.h
TAM_DESC_ID = 23
TAM_DESC_YEAR = 27
TAM_DESC_FIN_LOSS = 28
TAM_DESC_COUNTRY = 26
TAM_DESC_TYPE = 38
TAM_DESC_TGT_IND = 38
TAM_DESC_DEF = 67

class Header:
    def __init__(self):
        self.status = '0'
        self.topo = -1
        self.proxByteOffset = 0
        self.nroRegArq = 0
        self.nroRegRem = 0
        self.descreveIdentificador = ""
        self.descreveYear = ""
        self.descreveFinancialLoss = ""
        self.codDescreveCountry = '1'
        self.descreveCountry = ""
        self.codDescreveType = '2'
        self.descreveType = ""
        self.codDescreveTargetIndustry = '3'
        self.descreveTargetIndustry = ""
        self.codDescreveDefense = '4'
        self.descreveDefense = ""

class Record:
    def __init__(self):
        self.removido = 0
        self.tamanhoRegistro = 0
        self.prox = -1
        self.idAttack = 0
        self.year = 0
        self.financialLoss = 0.0
        self.country = ""
        self.attackType = ""
        self.targetIndustry = ""
        self.defenseMechanism = ""

def read_header(bin_file):
    header = Header()
    
    # Read variable fields
    header.status = bin_file.read(1).decode('utf-8')
    header.topo = struct.unpack('q', bin_file.read(8))[0]  # long int (8 bytes)
    header.proxByteOffset = struct.unpack('q', bin_file.read(8))[0]  # long int (8 bytes)
    header.nroRegArq = struct.unpack('i', bin_file.read(4))[0]  # int (4 bytes)
    header.nroRegRem = struct.unpack('i', bin_file.read(4))[0]  # int (4 bytes)
    
    # Read semantic fields
    header.descreveIdentificador = bin_file.read(TAM_DESC_ID).decode('utf-8').rstrip('\0')
    header.descreveYear = bin_file.read(TAM_DESC_YEAR).decode('utf-8').rstrip('\0')
    header.descreveFinancialLoss = bin_file.read(TAM_DESC_FIN_LOSS).decode('utf-8').rstrip('\0')
    
    header.codDescreveCountry = bin_file.read(1).decode('utf-8')
    header.descreveCountry = bin_file.read(TAM_DESC_COUNTRY).decode('utf-8').rstrip('\0')
    
    header.codDescreveType = bin_file.read(1).decode('utf-8')
    header.descreveType = bin_file.read(TAM_DESC_TYPE).decode('utf-8').rstrip('\0')
    
    header.codDescreveTargetIndustry = bin_file.read(1).decode('utf-8')
    header.descreveTargetIndustry = bin_file.read(TAM_DESC_TGT_IND).decode('utf-8').rstrip('\0')
    
    header.codDescreveDefense = bin_file.read(1).decode('utf-8')
    header.descreveDefense = bin_file.read(TAM_DESC_DEF).decode('utf-8').rstrip('\0')
    
    return header

def read_variable_string(bin_file):
    """Read variable length string field that may be empty or have content with identifier and delimiter."""
    result = ""
    
    # Read first byte to check if it's a field identifier or a delimiter
    first_byte = bin_file.read(1)
    if not first_byte or first_byte == b'|':
        # Empty field, just return empty string
        return ""
    
    # It's a field identifier, read the actual content
    field_id = first_byte.decode('utf-8')
    
    # Read until we find the delimiter '|'
    byte = bin_file.read(1)
    while byte and byte != b'|':
        result += byte.decode('utf-8')
        byte = bin_file.read(1)
    
    return result

def read_record(bin_file):
    record = Record()
    
    # Try to read the first field of the record
    removido_bytes = bin_file.read(4)
    if not removido_bytes or len(removido_bytes) < 4:
        return None  # End of file or corrupted record
    
    # Read fixed-size fields
    record.removido = struct.unpack('i', removido_bytes)[0]  # int (4 bytes)
    record.tamanhoRegistro = struct.unpack('i', bin_file.read(4))[0]  # int (4 bytes)
    record.prox = struct.unpack('q', bin_file.read(8))[0]  # long int (8 bytes)
    record.idAttack = struct.unpack('i', bin_file.read(4))[0]  # int (4 bytes)
    record.year = struct.unpack('i', bin_file.read(4))[0]  # int (4 bytes)
    record.financialLoss = struct.unpack('f', bin_file.read(4))[0]  # float (4 bytes)
    
    # Read variable-length fields that may be empty
    record.country = read_variable_string(bin_file)
    record.attackType = read_variable_string(bin_file)
    record.targetIndustry = read_variable_string(bin_file)
    record.defenseMechanism = read_variable_string(bin_file)
    
    return record

def bin_to_text(bin_filename, text_filename):
    with open(bin_filename, 'rb') as bin_file, open(text_filename, 'w') as text_file:
        # Read header
        header = read_header(bin_file)
        
        # Write header information to text file
        text_file.write("==== HEADER INFORMATION ====\n")
        text_file.write(f"Status: {header.status}\n")
        text_file.write(f"Top offset of removed records: {header.topo}\n")
        text_file.write(f"Next byte offset available: {header.proxByteOffset}\n")
        text_file.write(f"Number of records in file: {header.nroRegArq}\n")
        text_file.write(f"Number of removed records: {header.nroRegRem}\n")
        text_file.write("\n==== SEMANTIC DESCRIPTIONS ====\n")
        text_file.write(f"ID Attack: {header.descreveIdentificador}\n")
        text_file.write(f"Year: {header.descreveYear}\n")
        text_file.write(f"Financial Loss: {header.descreveFinancialLoss}\n")
        text_file.write(f"Country ({header.codDescreveCountry}): {header.descreveCountry}\n")
        text_file.write(f"Attack Type ({header.codDescreveType}): {header.descreveType}\n")
        text_file.write(f"Target Industry ({header.codDescreveTargetIndustry}): {header.descreveTargetIndustry}\n")
        text_file.write(f"Defense Mechanism ({header.codDescreveDefense}): {header.descreveDefense}\n")
        
        # Write records
        text_file.write("\n==== RECORDS ====\n")
        record_num = 1
        record_offset = bin_file.tell()  # Get current position which should be the first record
        
        while True:
            record = read_record(bin_file)
            if record is None:
                break
                
            text_file.write(f"\n--- Record #{record_num} (Offset: {record_offset}) ---\n")
            text_file.write(f"Removed: {'Yes' if record.removido else 'No'}\n")
            text_file.write(f"Record size: {record.tamanhoRegistro} bytes\n")
            text_file.write(f"Next removed record: {record.prox}\n")
            text_file.write(f"Attack ID: {record.idAttack}\n")
            text_file.write(f"Year: {record.year}\n")
            text_file.write(f"Financial Loss: ${record.financialLoss:.2f}\n")
            text_file.write(f"Country: {record.country if record.country else '[Empty]'}\n")
            text_file.write(f"Attack Type: {record.attackType if record.attackType else '[Empty]'}\n")
            text_file.write(f"Target Industry: {record.targetIndustry if record.targetIndustry else '[Empty]'}\n")
            text_file.write(f"Defense Mechanism: {record.defenseMechanism if record.defenseMechanism else '[Empty]'}\n")
            
            record_num += 1
            record_offset = bin_file.tell()  # Update offset for the next record
            
    print(f"Successfully converted {bin_filename} to {text_filename}")

def print_usage():
    print("Usage: python bin_to_text.py <input_bin_file> <output_text_file>")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print_usage()
        sys.exit(1)
        
    bin_filename = sys.argv[1]
    text_filename = sys.argv[2]
    
    try:
        bin_to_text(bin_filename, text_filename)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)