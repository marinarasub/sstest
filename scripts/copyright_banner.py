from datetime import datetime
from os import listdir, makedirs, path
from sys import argv
import argparse

################################################################################
#
# This file is a part of SSTest: A simple C++ testing library
# Copyright © 2022 David Lu
# 
# Python script for adding copyright banner to C++ source files 
#
################################################################################

# return list of lines wrapped to width
def wrap(s: str, max_width: int, tab_space: int=4, word_wrap=True) -> "list[str]":
    if (max_width < 1): return []

    lines = []
    strlen = len(s)
    curr_start = 0 # last unpushed index
    curr_end = 0 # current index

    while curr_end < strlen:
        # in each iteration of the loop, we want to push 1 line

        # peek ahead curr start until next non whitespace or line break to set curr_start
        while curr_start < strlen:
            c = s[curr_start]
            if c == '\t':
                curr_start += 1
            elif c == ' ':
                curr_start += 1
            else:
                break
        # curr start should now be at non whitespace char

        last_nospace = curr_start
        last_space = curr_start - 1
        curr_end = curr_start
        line_len = 0 # the logical line length, not actual number of chars
        # peek ahead curr end until newline or max line length
        while curr_end < strlen:
            c = s[curr_end]
            # handle line splitting
            if c == '\n':
                curr_end += 1
                break
            elif line_len >= max_width:
                # if not a natural break, we want to check if it is possible not to break across a word (rewind)
                if word_wrap:
                    if last_nospace == curr_end - 1 and last_space >= curr_start:
                        curr_end = last_space
                break
            elif curr_end == strlen - 1:
                curr_end += 1
                break

            # else advance characters in line
            if c == '\t':
                # actually, tab should be multiple of tab_space TODO
                last_space = curr_end
                line_len += tab_space
            elif c == ' ':
                last_space = curr_end
                line_len += 1
            else:
                last_nospace = curr_end
                line_len += 1

            curr_end += 1

        if c == '\n':
            line = s[ curr_start : curr_end ]
        else:
            line = s[ curr_start : curr_end ] + '\n'
        lines.append(line)

        curr_start = curr_end

    return lines


MIT_LICENSE_UTF8 = 'Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.'
MIT_LICENSE = 'Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.'

def generate_mit_license(year=datetime.utcnow().year, author="$AUTHOR") -> str:
    return f'Copyright (c) {year} {author}\n\n' + MIT_LICENSE

BANNER_MARK = '//**\n' # use this to determine whether banner exists or not

# /***************************************************************************//**
# * $PROJECT
# * 
# * $DESCRIPTION
# *
# * 
# * $LICENSE
# * 
# *******************************************************************************/
def generate_banner(project_name="$PROJECT", desc="$DESCRIPTION", license_text="", width=80) -> str:
    MIN_WIDTH = 10
    if width < MIN_WIDTH: width = MIN_WIDTH
    start_line =        '/' + ('*' * (width-4-1)) + BANNER_MARK
    end_line =          ('*' * (width-1)) + '/\n'

    project_name_line = f"{project_name}\n"
    desc_line =         f"{desc}\n"
    license_line =      f"{license_text}\n"

    content_str = \
        project_name_line + \
        '\n' + \
        desc_line + \
        '\n\n' + \
        license_line
    content_wrap = wrap(content_str, width-2)

    banner_str = start_line
    for line in content_wrap:
        banner_str = banner_str + "* " + line
    banner_str += '* \n'
    banner_str += end_line
    return banner_str

def is_h_file(file_name: str):
    return file_name.endswith(".h") or file_name.endswith(".hpp")

def is_cxx_file(file_name: str):
    return file_name.endswith(".cpp") or file_name.endswith(".cxx") or file_name.endswith(".cc")

def is_c_file(file_name: str):
    return file_name.endswith(".c")

def is_cxx_source_file(file_name: str):
    return is_h_file(file_name) or is_cxx_file(file_name) or is_c_file(file_name)

class run_config:
    def __init__(self):
        self.verbose = False
        self.dryrun = False
        self.sources = []
        self.resursive = False
        self.license_path = ""
        self.output_path = ""
        self.banner_width = 80

def discover(sources: "list[str]", recursive: bool, verbose: bool, is_root: bool=True):
    ret = set()
    for src in sources:
        if is_root:
            print(f"Scanning directory {src}")
        if not is_root and verbose:
            print(f"Scanning subdirectory {src}")
            
        dirents = listdir(src)
        files = filter(lambda dirent: path.isfile(path.join(src, dirent)), dirents)
        dirs = filter(lambda dirent: path.isdir(path.join(src, dirent)), dirents)

        for f in files:
            if is_cxx_source_file(f):
                if verbose:
                    print(f"\tFound source file: {f}")
                ret.add( path.normpath(path.join(src, f)) )
        
        if recursive:
            ret.update( discover([path.join(src, d) for d in dirs], recursive, verbose, False) )

    return ret


def stitch_banner(path, banner: str, dryrun: bool, verbose: bool):
    if verbose:
            if dryrun:
                print(f"Adding banner to {path} (dryrun)")
            else:
                print(f"Adding banner to {path}")
    if dryrun:
        return
    with open(path, "r+") as f:
        orig = ""
        # look for previous banner to overwrite
        lines = f.readlines()
        n = len(lines)
        i = 0
        if n > 0:
            if lines[0].endswith(BANNER_MARK):
                while i < n:
                    # read until non banner
                    l = lines[i]
                    if not l.isspace() and "*" not in l:
                        break
                    else:
                        lines[i] = ""
                    i += 1
        # add in the rest
        f.truncate(0)
        f.seek(0)
        f.write(banner)
        f.write('\n')
        while i < n:
            f.write(lines[i])
            i += 1

def run(config: run_config):
    year = datetime.utcnow().year
    author = 'David Lu'
    title = "SSTest: A C++ Testing Library"
    description = "This software is distributed free of charge, under the MIT License."

    files = discover(config.sources, config.resursive, config.verbose)
    print(f"Found {len(files)} source files total")


    if config.license_path:
        with open(config.license_path, "r") as f:
            license_text = f.read()
            print(f"Read license from {config.license_path}")
    else:
        license_text = generate_mit_license(year, author)

    banner = generate_banner( \
        title, \
        description, \
        license_text, \
        config.banner_width)

    if config.verbose: 
        print("Generated banner:\n\n" + banner + '\n')
    if config.output_path:
        with open(config.output_path, "w") as f:
            f.write(banner)
        print(f"Output to {config.output_path}")

    if config.dryrun:
        print(f"Adding banner to {len(files)} files (dryrun)")
    else:
        print(f"Adding banner to {len(files)} files")
    for f in files:
        stitch_banner(f, banner, config.dryrun, config.verbose)


def main(argv: "list[str]") -> int:
    parser = argparse.ArgumentParser(description="Generate and add copyright banner to all source and header files")
    parser.add_argument("-v", "--verbose", action="store_true", help="enable verbose messages")
    parser.add_argument("-d", "--dryrun", action="store_true", help="display what would happen at each step without actually doing so")
    parser.add_argument("-s", "--source", nargs='+', help="directores to discover source files from")
    parser.add_argument("-r", "--recursive", action="store_true", help="discover files in source directories recursively")
    parser.add_argument("-i", "--license", help="path to input license file")
    parser.add_argument("-o", "--output", help="output file with banner text")
    parser.add_argument("--width", type=int, default=80, help="specify banner width")
    args = parser.parse_args(argv[1:])

    config = run_config()
    config.verbose = args.verbose
    config.dryrun = args.dryrun
    config.sources = [] if args.source == None else args.source
    config.resursive = args.recursive
    config.license_path = args.license
    config.output_path = args.output
    config.banner_width = args.width

    run(config)

    return 0

if __name__ == '__main__':
    exit(main(argv))