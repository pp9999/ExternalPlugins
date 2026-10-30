"""Compare the plugin SDK with a MemoryError checkout without compiling either DLL."""
import argparse
from collections import Counter
from pathlib import Path
import re
import sys

# This checker covers the facade's single-line LIBRARY_API declarations and
# named type bodies; it does not preprocess C++ or inspect a built import library.
LEX = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', re.S)


def masked(text):
    return LEX.sub(lambda m: ''.join('\n' if c == '\n' else ' ' for c in m[0]), text)


def normalized(text):
    text = LEX.sub(lambda m: ' ' if m[0].startswith('/') else m[0], text)
    return re.sub(r'\s+', '', text)


def blocks(text):
    mask = masked(text)
    result = []
    for match in re.finditer(r'\b(namespace|class|struct)\s+(\w+)\s*\{', mask):
        depth, end = 1, match.end()
        while depth and end < len(mask):
            depth += (mask[end] == '{') - (mask[end] == '}')
            end += 1
        if depth:
            raise ValueError('Unclosed type or namespace: ' + match[2])
        result.append((match[2], match.start(), match.end(), end))
    return result


def declarations(text):
    mask, scopes = masked(text), blocks(text)
    result = []
    for match in re.finditer(r'(?m)^[ \t]*(?:template\s+\w+\s+)?LIBRARY_API\b[^\n]*', text):
        if not mask[match.start():match.end()].strip():
            continue
        line = match[0]
        end = line.rfind(';')
        if end < 0:
            raise ValueError('Unsupported multiline export: ' + line)
        decl = line[:end + 1].strip()
        owners = sorted((b for b in scopes if b[2] <= match.start() < b[3]), key=lambda b: b[1])
        scope = '::'.join(b[0] for b in owners)
        if re.search(r'\binline\b', decl):
            # SDK imports must not carry the host's inline variable definition.
            decl = re.split(r'\s*=|\{|\(', decl)[0].rstrip('; ').strip()
            decl = re.sub(r'\binline\s+', '', decl) + ';'
        result.append((scope, normalized(decl)))
    return Counter(result)


def type_body(text, name):
    block = next(b for b in blocks(text) if b[0] == name)
    return normalized(text[block[1]:block[3]])


def check(host_root, sdk_root):
    host = host_root / 'MemoryError'
    sdk = sdk_root / 'ExternalPlugins/source'
    read = lambda p: p.read_text(encoding='utf-8', errors='replace')
    main = read(host / 'MemoryError.h')
    host_exports = sum((declarations(read(host / f)) for f in
                        ['MemoryError.h', 'Detours.h', 'MemoryReadFun.h']), Counter())
    api = read(sdk / 'API.h')
    sdk_exports = declarations(api)
    errors = []
    for label, differences in [('Missing/changed host declaration', host_exports - sdk_exports),
                               ('Unsupported/changed SDK import', sdk_exports - host_exports)]:
        for (scope, decl), count in sorted(differences.items()):
            errors.append(f'{label}: {scope or "global"}: {decl} (x{count})')
    marker = '// Define a variant type for items,'
    if normalized(read(host / 'MemStructs1.h').split(marker, 1)[1]) != normalized(read(sdk / 'Structs.h').split(marker, 1)[1]):
        errors.append('Shared Structs.h types differ from MemStructs1.h')
    for name in ['ThreadSafeLogStream', 'TickInfo', 'KeyHoldInfo', 'Debug_Text', 'TickTimer']:
        try:
            if type_body(main, name) != type_body(api, name):
                errors.append('API.h type differs: ' + name)
        except StopIteration:
            errors.append('API.h type missing: ' + name)
    for filename in ['GrandExchange.h', 'Equipment.h', 'Inventory.h', 'Interact.h', 'RenderEvent.hpp']:
        host_text, sdk_text = read(host / filename), read(sdk / filename)
        # Compare every host type body, including nested structs and enums' members.
        for name, *_ in blocks(host_text):
            try:
                if type_body(host_text, name) != type_body(sdk_text, name):
                    errors.append(filename + ': type differs: ' + name)
            except StopIteration:
                errors.append(filename + ': missing type: ' + name)
        try:
            if declarations(host_text) != declarations(sdk_text):
                errors.append(filename + ': exported methods differ')
        except ValueError as error:
            errors.append(filename + ': ' + str(error))
    if errors:
        print('\n'.join(errors))
        return 1
    print(f'OK: {sum(host_exports.values())} facade declarations, shared structs, logging/tick types and 5 domain headers match the host.')
    print('Import-library compatibility and runtime behavior still require a matching developer build.')
    return 0


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--host', type=Path, required=True, help='MemoryError repository root')
    parser.add_argument('--sdk', type=Path, default=Path(__file__).resolve().parents[1], help='ExternalPluginsMM repository root')
    args = parser.parse_args()
    sys.exit(check(args.host, args.sdk))
