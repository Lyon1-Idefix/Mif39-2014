using System;

using DesignPatterns;

namespace FuzzyLogicSolver
{
	public class Object : INamedClass
	{
		public static UInt64 currentID = 0;
		public UInt64 ID;
		public string ClassName { get; set; }
		public Object ()
		{
			string tmp = GetType().Name;
			ClassName = tmp;
			ID = currentID++;
		}
	}
}

