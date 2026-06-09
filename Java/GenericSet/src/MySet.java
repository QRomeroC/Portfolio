import java.util.ArrayList;
import java.util.Iterator;

//Author: Cesar D. Quihuis-Romero

public class MySet<T> implements GenericSet<T>, Iterable<T> {
	private ArrayList<T> set = new ArrayList<T>();
	private int size = 0;

	public T get(int index) {
		return set.get(index);
	}

	@Override
	public boolean isEmpty() {
		return size == 0;
	}

	@Override
	public int size() {
		return size;
	}

	@Override
	public boolean add(T element) {
		if (contains(element)) {
			return false;
		} else {
			set.add(element);
			size++;
			return true;
		}
	}

	@Override
	public boolean contains(T element) {
		for (Object el : set) {
			if (el.equals(element)) {
				return true;
			}
		}
		return false;
	}

	@Override
	public boolean remove(T element) {
		if (this.isEmpty()) {
			return false;
		}
		if (this.contains(element)) {
			ArrayList<T> temp = new ArrayList<T>();
			int indexToRemove = set.indexOf(element);
			if (indexToRemove == 0) {
				for (int index = 1; index < size; index++) {
					temp.add(set.get(index));
				}
			} else if (indexToRemove == size - 1) {
				for (int index = 0; index < size - 1; index++) {
					temp.add(set.get(index));
				}
			} else {
				for (int index = 0; index < indexToRemove; index++) {
					temp.add(set.get(index));
				}
				for (int index = indexToRemove + 1; index < size; index++) {
					temp.add(set.get(index));
				}
			}
			set = temp;
			size--;
			return true;
		} else {
			return false;
		}

	}

	@Override
	public Iterator<T> iterator() {
		return new Iter<T>();
	}

	@SuppressWarnings("hiding")
	private class Iter<T> implements Iterator<T> {
		int curr = 0;

		@Override
		public boolean hasNext() {
			return curr < size;
		}

		@Override
		public T next() {
			@SuppressWarnings("unchecked")
			T obj = (T) set.get(curr);
			curr++;
			return obj;
		}

	}
}
