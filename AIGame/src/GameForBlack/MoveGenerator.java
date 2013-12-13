package GameForBlack;
import java.util.ArrayList;

public class MoveGenerator {

	public static ArrayList<StringBuffer> GenerateMoveOpening(
			StringBuffer board, char WorB) {
		// System.out.println("EnterInto the GenerateMoveOpening ");
		return GenerateAdd(board, WorB);

	}

	public static ArrayList<StringBuffer> GenerateMovesMidgameEndgame(
			StringBuffer board, char WorB) {
		int numWorBPieces = 0;
		int numnoWorBPieces = 0;
		char noWorB;
		if (WorB == 'W') {
			noWorB = 'B';
		} else {
			noWorB = 'W';
		}
		for (int location = 0; location < 23; location++) {
			if (board.charAt(location) == WorB) {
				numWorBPieces++;
			} else if (board.charAt(location) == noWorB) {
				numnoWorBPieces++;
			}
		}

		if (numWorBPieces == 3) {
			return GenerateHopping(board, WorB);
		} else {
			return GenerateMove(board, WorB);
		}

	}

	public static ArrayList<StringBuffer> GenerateMove(StringBuffer board,
			char WorB) {
		// TODO Auto-generated method stub
		String info = new String("before move "+ board+" color is " + WorB);
		ArrayList<StringBuffer> boardlist = new ArrayList<StringBuffer>();

		for (int location = 0; location < 23; location++) {
			if (board.charAt(location) == WorB) {
				int[] neighbour = null;
				neighbour = GetNeighbors(location, board);
				for (int location_neighbour = 0; (location_neighbour < 4)
						&& (neighbour[location_neighbour] != -1); location_neighbour++) {
					int j = neighbour[location_neighbour];

					if (board.charAt(j) == 'x') {
						StringBuffer newboard = new StringBuffer(board);
						newboard.setCharAt(location, 'x');
						newboard.setCharAt(j, WorB);
						if (closeMill(j, newboard)) {
							boardlist = GenerateRemove(newboard, boardlist,
									WorB);
						} else {
							boardlist.add(newboard);
						}
					}
				}
			}
		}
		return boardlist;
	}

	private static int[] GetNeighbors(int location, StringBuffer board) {
		// TODO Auto-generated method stub
		int[] my_neighbors = new int[4];
		my_neighbors[0] = 1;
		my_neighbors[1] = 3;
		my_neighbors[2] = 8;
		my_neighbors[3] = -1;
		switch (location) {
		case 0:
			my_neighbors[0] = 1;
			my_neighbors[1] = 3;
			my_neighbors[2] = 8;
			break;
		case 1:
			my_neighbors[0] = 0;
			my_neighbors[1] = 4;
			my_neighbors[2] = 2;
			break;
		case 2:
			my_neighbors[0] = 1;
			my_neighbors[1] = 5;
			my_neighbors[2] = 13;
			break;
		case 3:
			my_neighbors[0] = 0;
			my_neighbors[1] = 4;
			my_neighbors[2] = 5;
			my_neighbors[2] = 9;
			break;
		case 4:
			my_neighbors[0] = 1;
			my_neighbors[1] = 3;
			my_neighbors[2] = 5;
			break;
		case 5:
			my_neighbors[0] = 2;
			my_neighbors[1] = 4;
			my_neighbors[2] = 7;
			my_neighbors[3] = 12;
			break;
		case 6:
			my_neighbors[0] = 3;
			my_neighbors[1] = 7;
			my_neighbors[2] = 10;
			break;
		case 7:
			my_neighbors[0] = 5;
			my_neighbors[1] = 6;
			my_neighbors[2] = 11;
			break;
		case 8:
			my_neighbors[0] = 0;
			my_neighbors[1] = 9;
			my_neighbors[2] = 20;
			break;
		case 9:
			my_neighbors[0] = 3;
			my_neighbors[1] = 8;
			my_neighbors[2] = 10;
			my_neighbors[3] = 17;
			break;
		case 10:
			my_neighbors[0] = 6;
			my_neighbors[1] = 9;
			my_neighbors[2] = 14;
			break;
		case 11:
			my_neighbors[0] = 7;
			my_neighbors[1] = 12;
			my_neighbors[2] = 16;
			break;
		case 12:
			my_neighbors[0] = 5;
			my_neighbors[1] = 11;
			my_neighbors[2] = 13;
			my_neighbors[3] = 19;
			break;
		case 13:
			my_neighbors[0] = 2;
			my_neighbors[1] = 12;
			my_neighbors[2] = 22;
			break;
		case 14:
			my_neighbors[0] = 10;
			my_neighbors[1] = 15;
			my_neighbors[2] = 17;
			break;
		case 15:
			my_neighbors[0] = 14;
			my_neighbors[1] = 16;
			my_neighbors[2] = 18;
			break;
		case 16:
			my_neighbors[0] = 11;
			my_neighbors[1] = 15;
			my_neighbors[2] = 19;
			break;
		case 17:
			my_neighbors[0] = 9;
			my_neighbors[1] = 14;
			my_neighbors[2] = 18;
			my_neighbors[3] = 20;
			break;
		case 18:
			my_neighbors[0] = 15;
			my_neighbors[1] = 17;
			my_neighbors[2] = 19;
			my_neighbors[3] = 21;
			break;
		case 19:
			my_neighbors[0] = 12;
			my_neighbors[1] = 16;
			my_neighbors[2] = 18;
			my_neighbors[3] = 22;
			break;
		case 20:
			my_neighbors[0] = 8;
			my_neighbors[1] = 17;
			my_neighbors[2] = 21;
			break;
		case 21:
			my_neighbors[0] = 18;
			my_neighbors[1] = 20;
			my_neighbors[2] = 22;
			break;
		case 22:
			my_neighbors[0] = 13;
			my_neighbors[1] = 19;
			my_neighbors[2] = 21;
			break;
		default:
			break;
		}
		return my_neighbors;
	}

	private static ArrayList<StringBuffer> GenerateHopping(StringBuffer board,
			char WorB) {
		// TODO Auto-generated method stub
		ArrayList<StringBuffer> boardlist = new ArrayList<StringBuffer>();

		for (int location_a = 0; location_a < 23; location_a++) {
			if (board.charAt(location_a) == WorB) {
				for (int location_b = 0; location_b < 23; location_b++) {
					if (board.charAt(location_b) == 'x') {
						StringBuffer newboard = new StringBuffer(board);
						newboard.setCharAt(location_a, 'x');
						newboard.setCharAt(location_b, WorB);

						if (closeMill(location_b, newboard)) {
							boardlist = GenerateRemove(newboard, boardlist,
									WorB);
						} else {
							boardlist.add(newboard);
						}
					}
				}
			}
		}
		return boardlist;
	}

	public static ArrayList<StringBuffer> GenerateAdd(StringBuffer board,
			char WorB) {
		// TODO Auto-generated method stub
		// System.out.println("EnterInto the GenerateAdd " + board);

		ArrayList<StringBuffer> boardlist = new ArrayList<StringBuffer>();

		for (int location = 0; location < 23; location++) {
			if (board.charAt(location) == 'x') {
				StringBuffer newboard = new StringBuffer(board);
				newboard.setCharAt(location, WorB);
				if (closeMill(location, newboard)) {
					// System.out.println("closeMill returns true");
					boardlist = GenerateRemove(newboard, boardlist, WorB);
				} else {
					boardlist.add(newboard);
				}
			}
		}
		return boardlist;
	}

	public static ArrayList<StringBuffer> GenerateRemove(StringBuffer board,
			ArrayList<StringBuffer> boardlist, char WorB) {
		// TODO Auto-generated method stub
		// System.out.println("EnterInto the GenerateRemove " + board);
		char noWorB;
		if (WorB == 'B') {
			noWorB = 'W';
		} else {
			noWorB = 'B';

		}

		for (int location = 0; location < 23; location++) {
			if (board.charAt(location) == noWorB) {
				if (!closeMill(location, board)) {
					StringBuffer newboard = new StringBuffer(board);
					// System.out.println("before removing is "+ newboard);
					newboard.setCharAt(location, 'x');
					// System.out.println("after removing is "+ newboard);
					boardlist.add(newboard);
				}
			}
		}
		boardlist.add(board);
		return boardlist;

	}

	public static boolean closeMill(int location, StringBuffer newboardbuffer) {
		// TODO Auto-generated method stub
		// System.out.println("EnterInto the closeMill " + newboardbuffer +
		// " location "+location);
		char current = newboardbuffer.charAt(location);
		// System.out.println("current is " + current);
		switch (location) {
		case 0:
			if (((newboardbuffer.charAt(1) == current) && (newboardbuffer
					.charAt(2) == current))
					|| ((newboardbuffer.charAt(3) == current) && (newboardbuffer
							.charAt(6) == current))
					|| ((newboardbuffer.charAt(8) == current) && (newboardbuffer
							.charAt(20) == current))) {
				return true;
			} else {
				// System.out.println("this is false");

				return false;
			}
		case 1:
			if (((newboardbuffer.charAt(0) == current) && (newboardbuffer
					.charAt(2) == current))

			) {
				return true;
			} else {
				return false;
			}
		case 2:
			if (((newboardbuffer.charAt(0) == current) && (newboardbuffer
					.charAt(1) == current))
					|| ((newboardbuffer.charAt(13) == current) && (newboardbuffer
							.charAt(22) == current))
					|| ((newboardbuffer.charAt(5) == current) && (newboardbuffer
							.charAt(7) == current))) {
				return true;
			} else {
				return false;
			}
		case 3:
			if (((newboardbuffer.charAt(0) == current) && (newboardbuffer
					.charAt(6) == current))
					|| ((newboardbuffer.charAt(9) == current) && (newboardbuffer
							.charAt(17) == current))
					|| ((newboardbuffer.charAt(4) == current) && (newboardbuffer
							.charAt(5) == current))) {
				return true;
			} else {
				return false;
			}
		case 4:
			if (((newboardbuffer.charAt(3) == current) && (newboardbuffer
					.charAt(5) == current))) {
				return true;
			} else {
				return false;
			}
		case 5:
			if (((newboardbuffer.charAt(3) == current) && (newboardbuffer
					.charAt(4) == current))
					|| ((newboardbuffer.charAt(12) == current) && (newboardbuffer
							.charAt(19) == current))
					|| ((newboardbuffer.charAt(2) == current) && (newboardbuffer
							.charAt(7) == current))) {
				return true;
			} else {
				return false;
			}
		case 6:
			if (((newboardbuffer.charAt(0) == current) && (newboardbuffer
					.charAt(3) == current))
					|| ((newboardbuffer.charAt(10) == current) && (newboardbuffer
							.charAt(14) == current))) {
				return true;
			} else {
				return false;
			}
		case 7:
			if (((newboardbuffer.charAt(2) == current) && (newboardbuffer
					.charAt(5) == current))
					|| ((newboardbuffer.charAt(11) == current) && (newboardbuffer
							.charAt(16) == current))) {
				return true;
			} else {
				return false;
			}
		case 8:
			if (((newboardbuffer.charAt(9) == current) && (newboardbuffer
					.charAt(10) == current))
					|| ((newboardbuffer.charAt(0) == current) && (newboardbuffer
							.charAt(20) == current))) {
				return true;
			} else {
				return false;
			}
		case 9:
			if (((newboardbuffer.charAt(8) == current) && (newboardbuffer
					.charAt(10) == current))
					|| ((newboardbuffer.charAt(3) == current) && (newboardbuffer
							.charAt(17) == current))

			) {
				return true;
			} else {
				return false;
			}
		case 10:
			if (((newboardbuffer.charAt(8) == current) && (newboardbuffer
					.charAt(9) == current))
					|| ((newboardbuffer.charAt(6) == current) && (newboardbuffer
							.charAt(14) == current))) {
				return true;
			} else {
				return false;
			}
		case 11:
			if (((newboardbuffer.charAt(7) == current) && (newboardbuffer
					.charAt(16) == current))
					|| ((newboardbuffer.charAt(12) == current) && (newboardbuffer
							.charAt(13) == current))) {
				return true;
			} else {
				return false;
			}
		case 12:
			if (((newboardbuffer.charAt(11) == current) && (newboardbuffer
					.charAt(13) == current))
					|| ((newboardbuffer.charAt(5) == current) && (newboardbuffer
							.charAt(19) == current))) {
				return true;
			} else {
				return false;
			}
		case 13:
			if (((newboardbuffer.charAt(11) == current) && (newboardbuffer
					.charAt(12) == current))
					|| ((newboardbuffer.charAt(2) == current) && (newboardbuffer
							.charAt(22) == current))) {
				return true;
			} else {
				return false;
			}
		case 15:
			if (((newboardbuffer.charAt(14) == current) && (newboardbuffer
					.charAt(16) == current))
					|| ((newboardbuffer.charAt(18) == current) && (newboardbuffer
							.charAt(21) == current))) {
				return true;
			} else {
				return false;
			}
		case 16:
			if (((newboardbuffer.charAt(14) == current) && (newboardbuffer
					.charAt(15) == current))
					|| ((newboardbuffer.charAt(7) == current) && (newboardbuffer
							.charAt(11) == current))
					|| ((newboardbuffer.charAt(19) == current) && (newboardbuffer
							.charAt(22) == current))) {
				return true;
			} else {
				return false;
			}
		case 17:
			if (((newboardbuffer.charAt(18) == current) && (newboardbuffer
					.charAt(19) == current))
					|| ((newboardbuffer.charAt(14) == current) && (newboardbuffer
							.charAt(20) == current))
					|| ((newboardbuffer.charAt(3) == current) && (newboardbuffer
							.charAt(9) == current))) {
				return true;
			} else {
				return false;
			}
		case 18:
			if (((newboardbuffer.charAt(17) == current) && (newboardbuffer
					.charAt(19) == current))
					|| ((newboardbuffer.charAt(21) == current) && (newboardbuffer
							.charAt(15) == current))) {
				return true;
			} else {
				return false;
			}
		case 19:
			if (((newboardbuffer.charAt(17) == current) && (newboardbuffer
					.charAt(18) == current))
					|| ((newboardbuffer.charAt(16) == current) && (newboardbuffer
							.charAt(22) == current))
					|| ((newboardbuffer.charAt(5) == current) && (newboardbuffer
							.charAt(12) == current))) {
				//System.out.println("positon is 19");
				return true;
			} else {
				return false;
			}
		case 20:
			if (((newboardbuffer.charAt(0) == current) && (newboardbuffer
					.charAt(8) == current))
					|| ((newboardbuffer.charAt(14) == current) && (newboardbuffer
							.charAt(17) == current))
					|| ((newboardbuffer.charAt(21) == current) && (newboardbuffer
							.charAt(22) == current))) {
				return true;
			} else {
				return false;
			}
		case 21:
			if (((newboardbuffer.charAt(20) == current) && (newboardbuffer
					.charAt(22) == current))
					|| ((newboardbuffer.charAt(16) == current) && (newboardbuffer
							.charAt(19) == current))) {
				return true;
			} else {
				return false;
			}
		case 22:
			if (((newboardbuffer.charAt(20) == current) && (newboardbuffer
					.charAt(21) == current))
					|| ((newboardbuffer.charAt(16) == current) && (newboardbuffer
							.charAt(19) == current))
					|| ((newboardbuffer.charAt(2) == current) && (newboardbuffer
							.charAt(13) == current))) {
				return true;
			} else {
				return false;
			}
		default:
			return false;

		}
	}
}
