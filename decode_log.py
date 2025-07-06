import re
import sys

# Partial keycode map (X11)
keymap = {
    9: 'Esc', 10: '1', 11: '2', 12: '3', 13: '4', 14: '5', 15: '6',
    16: '7', 17: '8', 18: '9', 19: '0', 20: '-', 21: '=', 22: 'Backspace',
    23: 'Tab', 24: 'q', 25: 'w', 26: 'e', 27: 'r', 28: 't', 29: 'y',
    30: 'u', 31: 'i', 32: 'o', 33: 'p', 34: '[', 35: ']', 36: 'Enter',
    37: 'Ctrl', 38: 'a', 39: 's', 40: 'd', 41: 'f', 42: 'g', 43: 'h',
    44: 'j', 45: 'k', 46: 'l', 47: ';', 48: "'", 49: '`', 50: 'Shift',
    51: '\\', 52: 'z', 53: 'x', 54: 'c', 55: 'v', 56: 'b', 57: 'n',
    58: 'm', 59: ',', 60: '.', 61: '/', 62: 'Shift', 63: '*', 64: 'Alt',
    65: ' ', 66: 'CapsLock', 111: 'Up', 116: 'Down', 113: 'Left', 114: 'Right'
}

def decode_log(file_path):
    with open(file_path, 'r') as f:
        content = f.read()
    codes = re.findall(r'\[(\d+)\]', content)
    output = ''
    for code in codes:
        keycode = int(code)
        output += keymap.get(keycode, f'<{keycode}>')
    print(output)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python decode_log.py <log_file>")
    else:
        decode_log(sys.argv[1])
