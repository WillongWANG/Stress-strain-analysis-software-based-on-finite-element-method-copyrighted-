# 基于有限单元法的应力应变分析软件的设计与实现

国内外市场上有许多著名的力学有限元分析软件如Abaqus、SiPESC等。当聚焦到本科力学教育时，这些复杂的软件并不适合力学初学者使用学习，原因是对用户深厚的工程和数值模拟知识的要求，购买和维护这些软件的高成本以及软件固有的复杂度不适于初级力学问题的解决和学习。因此力学学习者所需要的是一款易于上手、功能完整且计算高效精准的力学软件，该软件能鼓励他们将力学理论运用于实际问题，在操作软件的过程中深入理解力学计算和有限元算法流程的原理，为以后的深入学习和实践能力打下基础。而国内高校的力学教育中正缺乏这样适合本科生的软件，缺乏理论用于实践的机会。

因此该毕业设计的目的在于设计一款功能齐全、图形用户界面友好、方便用户快速上手的软件，能解决大部分二维力学有限元分析问题，即是本科力学学习过程中会碰到的大部分问题。本毕设采用软件工程的科学方法，使用Qt设计用户界面，支持用户自定义力学模型和材料属性，运用Qt库函数在软件界面中可视化相应力学模型，设置足够的合法性检测，提高程序稳定性；运用Gmsh和Eigen库函数进行网格划分和有限元求解并采用广度优先遍历优化有限元算法，提升软件计算效率和精度；最后运用Qt库函数可视化计算出的位移和应力应变云图。在二维有限元分析领域，该软件较市面上成熟的力学分析软件如Abaqus操作更为简易，计算更加高效（计算过程小于1秒），并得到较为精确的应力应变值和云图。这种简易的软件可以节省力学初学者的使用成本和时间，降低学习门槛，提升学习兴趣和效率。

有限元假设单元内结点只与其横纵坐标有关，并由此构造形函数，对于单元结点不同的个数和位置，相应的形函数也不同，但其中参数必须满足对称性、完备性、位移变形相容性。本软件的创新之处在于提供两种有限元模型，三结点和六结点三角形单元，进行计算求解，便于比较两种算法的精度和效率，探讨高阶单元的优劣。

__软件设计逻辑、使用说明、用例测试参见[基于有限单元法的应力应变分析软件的设计与实现](https://github.com/WillongWANG/Stress-strain-analysis-software-based-on-finite-element-method-copyrighted-/blob/main/%E5%9F%BA%E4%BA%8E%E6%9C%89%E9%99%90%E5%8D%95%E5%85%83%E6%B3%95%E7%9A%84%E5%BA%94%E5%8A%9B%E5%BA%94%E5%8F%98%E5%88%86%E6%9E%90%E8%BD%AF%E4%BB%B6%E7%9A%84%E8%AE%BE%E8%AE%A1%E4%B8%8E%E5%AE%9E%E7%8E%B0.pdf)__

**运行程序：下载easyMechanics文件夹，运行其中easyMechanics.exe**

**运行项目源代码：**
