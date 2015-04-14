using System;

namespace FuzzyLogicSolver
{
	public class Operator : Expression
	{
		public Operator ()
		{
		}
		public FuzzyLogic.Operator theOperator { get; set; }
		public Expression Left { get; set; }
		public Expression Right { get; set; }
	}
}

