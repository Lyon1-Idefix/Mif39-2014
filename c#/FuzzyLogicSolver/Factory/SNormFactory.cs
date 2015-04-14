using System;

using DesignPatterns;

namespace FuzzyLogicSolver
{
	public class SNormFactory : Factory < SNorm >
	{
		static SNormFactory Instance { get; set; }
		public static SNormFactory Get () {
			if ( Instance == null )
				Instance = new SNormFactory ();
			return Instance;
		}
		protected SNormFactory ()
		{
			Register < Maximum > (Maximum.Builder);
		}
	}
}

