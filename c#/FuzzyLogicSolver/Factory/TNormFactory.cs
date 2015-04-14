using System;

using DesignPatterns;

namespace FuzzyLogicSolver
{
	public class TNormFactory : Factory < TNorm >
	{
		static TNormFactory Instance { get; set; }
		public static TNormFactory Get () {
			if ( Instance == null )
				Instance = new TNormFactory ();
			return Instance;
		}
		protected TNormFactory ()
		{
			Register < Minimum > (Minimum.Builder);
		}
	}
}

