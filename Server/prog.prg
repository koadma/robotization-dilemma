<root>
	<assign>
		<to>
			<index>
				<arg>
					<var var="a">
					</var>
				</arg>
				<ind>
					<constant type="num">
						0
					</constant>
				</ind>
			</index>
		</to>
		<val>
			<constant type="num">
				5
			</constant>
		</val>
	</assign>
	<assign>
		<to>
			<var var="b">
			</var>
		</to>
		<val>
			<index>
				<arg>
					<var var="a">
					</var>
				</arg>
				<ind>
					<constant type="num">
						0
					</constant>
				</ind>
			</index>
		</val>
	</assign>
	<assign>
		<to>
			<var var="b">
			</var>
		</to>
		<val>
			<math oper="+">
				<lhs>
					<var var="b">
					</var>
				</lhs>
				<rhs>
					<constant type="num">
						2
					</constant>
				</rhs>
			</math>
		</val>
	</assign>
</root>