using System;

namespace FuzzyLogicSolver
{
	public class Maximum : SNorm
	{
		public override float Compute ( float a, float b ) {
			if (a < b)
				return b;
			return a;
		}
		protected Maximum () {
		}
		public static SNorm Builder (params string[] arguments) {
			return new Maximum ();
		}
	}
}

