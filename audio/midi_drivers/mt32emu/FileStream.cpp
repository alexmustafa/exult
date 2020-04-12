/* Copyright (C) 2003, 2004, 2005, 2006, 2008, 2009 Dean Beeler, Jerome Fisher
 * Copyright (C) 2011-2016 Dean Beeler, Jerome Fisher, Sergey V. Mikayev
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "internals.h"

#include "FileStream.h"

namespace MT32Emu {

using std::ios_base;

FileStream::FileStream() : ifsp(*new std::ifstream), data(nullptr), size(0)
{}

FileStream::~FileStream() {
	// destructor closes ifsp
	delete &ifsp;
	delete[] data;
}

size_t FileStream::getSize() {
	if (size != 0) {
		return size;
	}
	if (!ifsp.is_open()) {
		return 0;
	}
	ifsp.seekg(0, ios_base::end);
	size = size_t(ifsp.tellg());
	return size;
}

const Bit8u *FileStream::getData() {
	if (data != nullptr) {
		return data;
	}
	if (!ifsp.is_open()) {
		return nullptr;
	}
	if (getSize() == 0) {
		return nullptr;
	}
	Bit8u *fileData = new Bit8u[size];
	if (fileData == nullptr) {
		return nullptr;
	}
	ifsp.seekg(0);
	ifsp.read(reinterpret_cast<char *>(fileData), std::streamsize(size));
	if (size_t(ifsp.tellg()) != size) {
		delete[] fileData;
		return nullptr;
	}
	data = fileData;
	close();
	return data;
}

bool FileStream::open(const char *filename) {
	ifsp.clear();
	ifsp.open(filename, ios_base::in | ios_base::binary);
	return !ifsp.fail();
}

void FileStream::close() {
	ifsp.close();
	ifsp.clear();
}

} // namespace MT32Emu
