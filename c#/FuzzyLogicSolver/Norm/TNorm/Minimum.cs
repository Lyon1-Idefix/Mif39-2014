﻿// ------------------------------------------------------------------------------
//  <autogenerated>
//      This code was generated by a tool.
//      Mono Runtime Version: 4.0.30319.1
// 
//      Changes to this file may cause incorrect behavior and will be lost if 
//      the code is regenerated.
//  </autogenerated>
// ------------------------------------------------------------------------------
using System;

namespace FuzzyLogicSolver
{
	public class Minimum : TNorm
	{
		public override float Compute ( float a, float b ) {
			if (a > b)
				return b;
			return a;
		}
		protected Minimum () {
		}
		public static TNorm Builder (params string[] arguments) {
			return new Minimum ();
		}
	}
}

