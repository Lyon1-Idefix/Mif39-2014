using System;

using DesignPatterns;

namespace FuzzyLogicSolver
{
	public class DefuzzyfierFactory : Factory < Defuzzyfier >
	{
		static DefuzzyfierFactory Instance { get; set; }
		public static DefuzzyfierFactory Get () {
			if ( Instance == null )
				Instance = new DefuzzyfierFactory ();
			return Instance;
		}
		protected DefuzzyfierFactory ()
		{
			Register < Centroid > (Centroid.Builder);
		}
	}
}

