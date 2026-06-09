//ArrList.java
//Author: Cesar D Quihuis-Romero
//Purpose: create a dynamically re-sizable list structure that is implemented by use of
//an Array object (which acts as a standard java array with additional required methods).
//
//private class variables are:
//intArray = Array Object : used as the underlying structure for our list
//size = Int : represents the number of elements in our list (count)
//start = Int : represents the index of the start of the list (expected to be occupied)(pointer to first element)
//end = Int : represents the index of the end of list (expected to be empty)(pointer to the next unoccupied position to append to).

public class ArrList {
	private Array intArray;
	private int size, start, end;

	// constructor
	public ArrList() {
		intArray = new Array(11);
		size = 0;
		start = 0;
		end = 0;
	}

	public void addLast(int num) {
		// addLast adds to end, then moves pointer.
		if (this.isFull()) {
			this.growArray();
		}
		// since end points to size (last list index + 1) or is "empty"
		// set val then move pointer
		intArray.setVal(end, num);
		end = incPointer(end);
		// update size
		size++;
	}

	public void addFirst(int num) {
		if (this.isFull()) {
			this.growArray();
		}
		// empty or not, as long as its not full
		// treat all adds the same move the pointer
		// and put the value.
		start = decPointer(start);
		intArray.setVal(start, num);
		// update size
		size++;
	}

	public void add(int i, int num) throws ListIndexOutOfBoundsException {

		if (i < 0 || i >= size) {
			throw new ListIndexOutOfBoundsException();
		}

		if (this.isFull()) {
			growArray();
		}
		// if i == 0 we can call addFirst
		if (i == 0) {
			addFirst(num);
			// if i == last list index (size - 1) add last
		} else if (i == size - 1) {
			addLast(num);
		} else {
			// otherwise start at the last list index (size - 1)
			// move values one spot to the right.
			for (int index = size - 1; index >= i; index--) {
				int targetIndex = (start + index) % intArray.length();
				int targetVal = intArray.getVal(targetIndex);
				int nextIndex = (start + (index + 1)) % intArray.length();
				intArray.setVal(nextIndex, targetVal);
			}
			size++;
			end = incPointer(end);

			int targetIndex = (start + i) % intArray.length();
			intArray.setVal(targetIndex, num);

		}
	}

	public int get(int i) throws ListIndexOutOfBoundsException {

		if (i < 0 || i >= size) {
			throw new ListIndexOutOfBoundsException();
		}

		int targetIndex = (start + i) % intArray.length();
		return intArray.getVal(targetIndex);

	}

	public int indexOf(int num) {

		if (this.isEmpty()) {
			return -1;
		}

		for (int i = 0; i < size; i++) {
			int targetIndex = (start + i) % intArray.length();
			// System.out.println("ArrList (106) targetIndex: " + targetIndex);
			// System.out.println("ArrList (107) start: " + this.start);

			if (intArray.getVal(targetIndex) == num) {
				return i;
			}
		}

		return -1;
	}

	public boolean contains(int num) {
		// since my index of should be working now...hopefully.
		// if indexOf(num) != -1 then num is in list else
		// return indexOf(num) returns -1 and (-1 != -1) is false
		// meaning false the list does not contain num.
		return indexOf(num) != -1;
	}

	public boolean isEmpty() {
		return size == 0;
	}

	public int lastIndexOf(int num) {
		//start at size - 1 and move left until you find num or until i = 0
		for (int i = size - 1; i >= 0; i--) {
			int targetIndex = (start + i) % intArray.length();
			if (intArray.getVal(targetIndex) == num) {
				return i;
			}
		}
		return -1;

	}

	public int removeFirst() throws EmptyListException {

		if (this.isEmpty()) {
			throw new EmptyListException();
		}

		int retEle = intArray.getVal(start);
		start = incPointer(start);
		size--;
		checkShrink();
		return retEle;
	}

	public int removeLast() throws EmptyListException {

		if (this.isEmpty()) {
			throw new EmptyListException();
		}
		// since end always points to the next empty position (iff the list isn't full)
		// end points to size but list has (size - 1) indices.
		// we have to move the pointer back first then retrieve the element!!!!!!
		end = decPointer(end);

		int retEle = intArray.getVal(end);

		size--;
		checkShrink();
		return retEle;
	}

	public int removeByIndex(int i) throws EmptyListException, ListIndexOutOfBoundsException {
		if (this.isEmpty()) {
			throw new EmptyListException();
		}

		if ((i < 0) || (i >= size)) {
			throw new ListIndexOutOfBoundsException();
		} else {
			if (i == 0) {
				return removeFirst();
			} else if (i == size - 1) {
				return removeLast();
			} else {
				int targetIndex = (start + i) % intArray.length();
				int retVal = intArray.getVal(targetIndex);

				for (int index = i; index < size; index++) {
					targetIndex = (start + index) % intArray.length();
					int nextIndex = ((start + index) + 1) % intArray.length();
					int nextVal = intArray.getVal(nextIndex);
					intArray.setVal(targetIndex, nextVal);
				}
				end = decPointer(end);
				size--;
				checkShrink();
				return retVal;
			}
		}

	}

	public boolean removeByValue(int num) throws EmptyListException {
		if (this.isEmpty()) {
			throw new EmptyListException();
		}
		// first let's get the indexOf (assuming my index of is good)
		int indexOf = this.indexOf(num);

		// if that index is -1 value doesn't exist
		if (indexOf == -1) {
			return false;
		}
		// since we now know that if we got to this point the number exists
		// we should be safe to try removeByIndex
		try {
			this.removeByIndex(indexOf);
		} catch (EmptyListException | ListIndexOutOfBoundsException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return true;
	}

	public void removeRange(int i, int j) throws EmptyListException, ListIndexOutOfBoundsException {
		if (this.isEmpty()) {
			throw new EmptyListException();
		}

		if ((i > j) || (i < 0 || j >= size)) {
			throw new ListIndexOutOfBoundsException();
		} else {
//tried running remove by index for value in range of i to j but problem
//			for (int index = 0; index <= (j - i); index++) {
//			removeByIndex(i);
			// trying a sliding window approach.
			// range is from (start + i) to (start + j)
			int targetRangeStart = (start + i) % intArray.length();
			int targetRangeEnd = (start + j) % intArray.length();
			//if the targetRangeEnd is equal to last element in list (size - 1)
			//then check where the start of the range is
			if (j == size -1) {
				//if range start is > then start then it is not equal to start
				//which means elements from start to targetRangeStart are valid 
				if (i > 0) {
					//make end of list = start of range to remove
					end = targetRangeStart;
					size = size - ((j - i) + 1);
				} else {
					//else if target range is not > start then it must be equal to start
					//which means were removing the whole list;
					start = 0;
					end = 0;
					size = 0;
					checkShrink();
				}
			} else {
				// could have done this recursively...should have done this recursively
				//get index right of j (right of last index of range to remove)
				int indexRightOfJ = (start + (j + 1)) % intArray.length();
				//get val at that index right of j
				int valueRightOfJ = intArray.getVal(indexRightOfJ);
				//while j is less then size (i.e j will get to size - 1)
				while (j < size) {
					//set the start of my range to the value to the right of my range
					intArray.setVal(targetRangeStart, valueRightOfJ);
					//move i and j
					i++;
					j++;
					//update ref to start of range and end of range in terms of new i and j
					targetRangeStart = (start + i) % intArray.length();
					targetRangeEnd = (start + j) % intArray.length();
					//calc new index and value right of j
					indexRightOfJ = (start + (j + 1)) % intArray.length();
					valueRightOfJ = intArray.getVal(indexRightOfJ);
					//loop back to start
				}
				//make end equal to final index of start of range 
				//(it will have been moved to a position right of valid number of list)
				end = targetRangeStart;
				//update size to size - ((j - i) + 1 )
				size = size - ((j - i) + 1);
				//check if i need to shrink
				checkShrink();
			}
		}
	}

	public int set(int index, int num) throws ListIndexOutOfBoundsException {
		if (index < 0 || index >= size) {
			throw new ListIndexOutOfBoundsException();
		}
		//same calc for nth index of list
		int targetIndex = (start + index) % intArray.length();
		int retEle = intArray.getVal(targetIndex);
		intArray.setVal(targetIndex, num);
		return retEle;
	}

	public int size() {
		return this.size;
	}

	public boolean isFull() {
		return size == intArray.length();
	}

	private void growArray() {
		//grow makes a new array object and copies over data
		Array newArray = new Array(this.intArray.length() * 2);

		for (int i = 0; i < size; i++) {
			int targetIndex = (start + i) % intArray.length();
			newArray.setVal(i, intArray.getVal(targetIndex));
		}

		intArray = newArray;
		start = 0;
		end = size;
	}

	private void checkShrink() {
		// i can reduce shrinkArray having to check if array length is at min len of 11.
		if (size <= ((this.intArray.length() / 4)) && (this.intArray.length() > 11)) {
			shrinkArray();
		}
	}

	private void shrinkArray() {
		//list grow make new array of size (len/2) and copy over
		Array newArray = new Array(this.intArray.length() / 2);

		for (int i = 0; i < size; i++) {
			int index = (start + i) % this.intArray.length();
			newArray.setVal(i, this.intArray.getVal(index));
		}
		
		intArray = newArray;
		start = 0;
		end = size;
	}

	private int incPointer(int pointer) {
		//just do the math on the value and assign the variable name to the function call
		return (pointer + 1) % intArray.length();
	}

	private int decPointer(int pointer) {
		// same as incPointer i can use (base + i) % len to calc my target
		// index of new start and new end i just have to add the length of the array
		// to offset for if start or end is 0 and we go negative.
		//same as int pointer do the math on the value and use assignment to adjust the correct pointer
		return ((pointer + intArray.length()) - 1) % intArray.length();
	}
	
	private int shiftPointer(int pointer, int shiftAmount) {
		return (pointer + shiftAmount) % intArray.length();
	}
	
	public int getStart() {
		return this.start;
	}

	public int getEnd() {
		return this.end;
	}

	public int getAccessCount() {
		return intArray.getAccessCount();
	}

	public void resetAccessCount() {
		intArray.resetAccessCount();
	}

}
