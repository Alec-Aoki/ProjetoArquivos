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
    # Save the starting position
    start_pos = bin_file.tell()
    
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
    
    # Calculate the header size
    header_size = bin_file.tell() - start_pos
    
    return header, header_size

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
        # Read header and get its size
        header, header_size = read_header(bin_file)
        
        # Write header information to text file
        text_file.write("==== HEADER INFORMATION ====\n")
        text_file.write(f"Status: {header.status}\n")
        text_file.write(f"Top offset of removed records: {header.topo}\n")
        text_file.write(f"Next byte offset from header: {header.proxByteOffset}\n")
        text_file.write(f"Number of records in file: {header.nroRegArq}\n")
        text_file.write(f"Number of removed records: {header.nroRegRem}\n")
        text_file.write(f"Header size: {header_size} bytes\n")
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
        
        # First record starts after the header
        first_record_offset = header_size
        current_offset = first_record_offset
        
        record_positions = []  # To store record positions for later calculation
        
        while True:
            record_start = bin_file.tell()
            record = read_record(bin_file)
            if record is None:
                break
            
            record_end = bin_file.tell()
            record_size = record_end - record_start
            record_positions.append((record_start, record_size))
                
            text_file.write(f"\n--- Record #{record_num} (Offset: {record_start}) ---\n")
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
        
        # Calculate the next free byte offset
        if record_positions:
            last_record_offset, last_record_size = record_positions[-1]
            calculated_next_offset = last_record_offset + last_record_size
        else:
            calculated_next_offset = header_size
        
        # Add a summary section
        text_file.write("\n==== SUMMARY ====\n")
        text_file.write(f"Next byte offset from header: {header.proxByteOffset}\n")
        text_file.write(f"Calculated next free byte offset: {calculated_next_offset}\n")
        text_file.write(f"Total records read: {record_num - 1}\n")
        
        # Check if there's a discrepancy
        if header.proxByteOffset != calculated_next_offset:
            text_file.write(f"WARNING: Discrepancy detected between header byte offset and calculated next free offset!\n")
            
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