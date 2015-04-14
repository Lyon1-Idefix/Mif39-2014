using System;

namespace FuzzyLogicSolver
{
	public class Variable : Object
	{
		public Variable ()
		{
		}
		public FuzzyLogic.Variable theVariable { get; set; }
		public CurveList Terms = new CurveList ();
	}
}

