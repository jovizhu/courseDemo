import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.RandomAccessFile;
import java.io.Reader;
import java.io.Writer;
import java.util.ArrayList;
import java.util.Iterator;

public class MinMaxOpening {

	public static String result = null;
	
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		// Readin the file

		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		String input = new String();
		try {
			input = in.readLine();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

		System.out.println(input.split(" ")[0]);
		String inputfilename = input.split(" ")[0];
		System.out.println(input.split(" ")[1]);
		String outputfilename = input.split(" ")[1];
		String deep  = input.split(" ")[2];
		System.out.println(input.split(" ")[2]);

		RandomAccessFile inputfile = null;
		RandomAccessFile outputfile = null;
		// inputfile = new String("board1.txt");

		String line = null;
		try {
			FileInputStream fin = new FileInputStream(inputfilename);

			BufferedReader reader = new BufferedReader(new InputStreamReader(
					fin));

			line = reader.readLine();

		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		StringBuffer board = new StringBuffer(line);
		int turns = new Integer(deep);
		System.out.println(turns);
		MinMaxOpening minmaxopening = new MinMaxOpening();
		System.out.println("Final MinMax is "+minmaxopening.MaxMin(board,
		turns));

		try {
			FileOutputStream fos = new FileOutputStream(outputfilename);
			Writer writer = new BufferedWriter(new OutputStreamWriter(fos));
			writer.write(minmaxopening.result);
			writer.flush();

		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	public int MaxMin(StringBuffer board, int turns) {
		
		StringBuffer info = new StringBuffer("Enter into MaxMin "+ board+" turns is " + turns);
		String result = null;
		if (turns == 0) {
			int maxmin = StaticEstimation.StaticEstimationForOpeining(board);
			
			return maxmin;
		}
		ArrayList<StringBuffer> boardlist = MoveGenerator.GenerateMoveOpening(
				board, 'W');

		Iterator<StringBuffer> itr = boardlist.iterator();
		turns--;
		int v = -10000;
		while (itr.hasNext()) {

			StringBuffer child_board = itr.next();
			// System.out.println("Calcualte for the "+ child_board +
			// " length is " + boardlist.size());

			int minmax = MinMax(child_board, turns);
			if (minmax > v) {
				v = minmax;
				result = new String(child_board);

			}

		}

		info.append("result is " +result);
		System.out.println(info);
		MinMaxOpening.result = new String(result);
		return v;
	}

	public int MinMax(StringBuffer board, int turns) {
		// TODO Auto-generated method stub
		StringBuffer info = new StringBuffer("Enter into MaxMin "+ board+" turns is " + turns);
		String result = null;
		
		if (turns == 0) {
			int minmax = StaticEstimation.StaticEstimationForOpeining(board);
			
			return minmax;
		}

		ArrayList<StringBuffer> boardlist = MoveGenerator.GenerateMoveOpening(
				board, 'B');

		Iterator<StringBuffer> itr = boardlist.iterator();
		turns--;
		int v = 10000;
		while (itr.hasNext()) {

			StringBuffer child_board = itr.next();
			
			int minmax = MaxMin(child_board, turns);
			if (minmax < v) {
				v = minmax;
				result = new String(child_board);
			}
		}

		info.append("result is " +result);
		System.out.println(info);
		return v;

	}

}
