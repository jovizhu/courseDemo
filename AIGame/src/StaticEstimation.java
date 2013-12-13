
public class StaticEstimation {

	public static int StaticEstimationForOpeining(StringBuffer board)
	{
			//System.out.println("EnterInto StatciEstimateForOpening " + board);
			int numWhitePieces = 0;
			int numBlackPieces = 0;
			for(int location =0; location <23;location ++)
			{
				if(board.charAt(location) == 'W')
				{
					numWhitePieces ++;
				}
				else if(board.charAt(location) == 'B')
				{
					numBlackPieces ++;
				}
			}
			
			//System.out.println("White pieces is "+ numWhitePieces);
			//System.out.println("Black pieces is "+ numBlackPieces);
			return numWhitePieces - numBlackPieces;
	}
	
	
	public static int StaticEstimateForMidgameEndgame(StringBuffer board)
	{
		int numWhitePieces = 0;
		int numBlackPieces = 0;
		
		for(int location =0; location <23;location ++)
		{
			if(board.charAt(location) == 'W')
			{
				numWhitePieces ++;
			}
			else if(board.charAt(location) == 'B')
			{
				numBlackPieces ++;
			}
		}
		
		int numBlackMoves = 23-numWhitePieces-numBlackPieces;
		
		if(numBlackPieces <= 2) 
			{
				//System.out.println("return is "+10000);
				return 10000;
			}
		else if(numWhitePieces <= 2)
		{
			//System.out.println("return is -"+10000);
			return -10000;
		}
		else if(numBlackMoves ==0)
		{
			//System.out.println("return is "+10000);
			return 10000;
		}
		else
		{
			//System.out.println(1000*(numWhitePieces - numBlackPieces) - numBlackMoves);
			return (1000*(numWhitePieces - numBlackPieces) - numBlackMoves);
		}
	}
}
