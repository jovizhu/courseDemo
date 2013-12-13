package GameForBlack;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.RandomAccessFile;
import java.io.Writer;
import java.util.ArrayList;
import java.util.Iterator;


public class AlphaBetaGame {
	public  static String result = null;
	public static int estimate_count = 0;
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

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
		
		AlphaBetaGame alphabetagame = new AlphaBetaGame();
		int alpha = -1000;
		int beta = 1000;
		System.out.println("Final MinMax is "+alphabetagame.AlphaBetaMaxMin(board, turns,  alpha,  beta));
		System.out.println("Board Position: "+ result);
		System.out.println("Positon Estimated: "+estimate_count);
		
		try {
			FileOutputStream fos = new FileOutputStream(outputfilename);
			Writer writer = new BufferedWriter(new OutputStreamWriter(fos));
			writer.write(alphabetagame.result);
			writer.flush();

		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	public int AlphaBetaMaxMin(StringBuffer board, int turns, int alpha, int beta)
	{
		//System.out.println("Enter into MaxMin "+ board+"turns is " + turns);
		StringBuffer info = new StringBuffer("Enter into AlphaBetaMaxMin "+ board+" turns is " + turns);
		String result = null;
		
		if(turns == 0)
		{
			int maxmin = StaticEstimation.StaticEstimateForMidgameEndgame(board);
			//System.out.println("the leaf for the opeining in maxmin " + maxmin);
			return maxmin;
		}
		ArrayList<StringBuffer> boardlist= MoveGenerator.GenerateMovesMidgameEndgame(board, 'W');
		
		Iterator<StringBuffer> itr = boardlist.iterator();
		turns --;
		int v = -10000;
		while(itr.hasNext())
		{
			
			estimate_count++;
			StringBuffer child_board = itr.next();
			//System.out.println("Calcualte for the "+ child_board + " length is " + boardlist.size());
			
			int minmax= AlphaBetaMinMax(child_board, turns, alpha, beta);
			if(minmax > v)
			{
				v = minmax;
				result = new String(child_board);
				
			}
			else
			{
				if(v > alpha)
				{
				alpha = v;
				}
			}
			
			
		}
		
		info.append("result is " +result);
		System.out.println(info);
		
		AlphaBetaGame.result  = new String(result);
		
		return v;
	}

	public int  AlphaBetaMinMax(StringBuffer board, int turns, int alpha, int beta) {
		// TODO Auto-generated method stub
		//System.out.println("Enter into MinMax "+ board+"turns is " + turns);
		StringBuffer info = new StringBuffer("Enter into AlphaBetaminMax "+ board+" turns is " + turns);
		String result = null;
		if(turns == 0)
		{
			int minmax = StaticEstimation.StaticEstimateForMidgameEndgame(board);
			//System.out.println("the leaf for the opeining in minmax " + minmax);
			return minmax;
		}
		
		ArrayList<StringBuffer> boardlist= MoveGenerator.GenerateMovesMidgameEndgame(board, 'B');
		
		Iterator<StringBuffer> itr = boardlist.iterator();
		turns--;
		int v = 10000;
		while(itr.hasNext())
		{
			estimate_count++;
			StringBuffer child_board = itr.next();
			//System.out.println(child_board.toString()+" is the child ");
			int minmax= AlphaBetaMaxMin(child_board, turns, alpha, beta);
			if(minmax < v)
			{
				v = minmax;
				result = new String(child_board);
			}
			else
			{
				if(v< beta)
				{
					beta = v;
				}
			}
		}
		
		info.append("result is " +result);
		System.out.println(info);
		return v;
	
	}
}
