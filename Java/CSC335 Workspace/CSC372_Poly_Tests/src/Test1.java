import java.util.Arrays;

public class Test1 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int first = addAll(1,2,3);
		double second = addAll(1.0,2.0,3.0);
		int[] array = {1,2,3};
		int third = addAll(array);
		System.out.println(first);
		System.out.println(second);
		System.out.println(Arrays.toString(array));
	}

	public static int addAll (int a, int b, int c) {
		// TODO Auto-generated method stub
		return a + b + c;
	}
	
	//public static double addAll (int a, int b, int c) {
		
	
	//}
	
	public static double addAll (double a, double b, double c) {
		return a + b + c;
	}
	
	//private static int addAll (int a, int b, int c) {
		
	//}
	
	public static int addAll (int[] array) {
		int sum = 0;
		for (int i = 0; i < array.length; i++) {
			sum += array[i];
		}
		return sum;
	}
	

}
