#!/usr/bin/env python

"""
Test suite for the Lgm_DateAndTime file

@author: Brian Larsen
@organization: LANL
@contact: balarsen@lanl.gov

@version: V1: 22-Dec-2010 (BAL)
"""


import unittest
import datetime
import ctypes

import numpy

import Lgm
from _Lgm import lib
import Lgm_DateAndTime
import Lgm_CTrans
from Lgm_Types import LgmDouble, LgmLong


class Lgm_DateAndTimeTests(unittest.TestCase):
    """
    Tests related to Lgm_DateAndTime
    @author: Brian Larsen
    @organization: LANL
    @contact: balarsen@lanl.gov

    @version: V1: 20-Dec-2010 (BAL)
    """

    def setUp(self):
        super(Lgm_DateAndTimeTests, self).setUp()

    def tearDown(self):
        super(Lgm_DateAndTimeTests, self).tearDown()

    #def test_DateAndTimeSize(self):
    #    """Lgm_DateAndTime shize should match c/python"""
    #    self.assertEqual(ctypes.sizeof(Lgm_DateAndTime.Lgm_DateAndTime),
    #                     lib.size_DateAndTime())

    def testLgm_DateAndTime(self):
        """Lgm_DateAndTime has a nLeapSecondDates, LeapSecondDates, LeapSecondJDs, LeapSeconds"""
        self.assertTrue(hasattr(Lgm_DateAndTime.Lgm_DateAndTime, 'nLeapSecondDates'))
        self.assertTrue(hasattr(Lgm_DateAndTime.Lgm_DateAndTime, 'LeapSecondDates'))
        self.assertTrue(hasattr(Lgm_DateAndTime.Lgm_DateAndTime, 'LeapSecondJDs'))
        self.assertTrue(hasattr(Lgm_DateAndTime.Lgm_DateAndTime, 'LeapSeconds'))


    def testLgm_DateAndTime_Type(self):
        """Lgm_DateAndTime input should be of correct types """
        b = LgmLong()
        c = LgmDouble()
        d = LgmDouble()
        ans = Lgm_DateAndTime.Lgm_DateAndTime(5, b, c, d)
        self.assertTrue(isinstance(ans.nLeapSecondDates, int))
        self.assertTrue(isinstance(ans.LeapSecondDates, int))
        self.assertTrue(isinstance(ans.LeapSecondJDs, float))
        self.assertTrue(isinstance(ans.LeapSeconds, float))

    def test_Lgm_LeapYear(self):
        """Lgm_LeapYear should give known output for known input"""
        leaps = [1600, 1604, 1608, 1612, 1616, 1620, 1624, 1628, 1632, 1636,
                 1640, 1644, 1648, 1652, 1656, 1660, 1664, 1668, 1672, 1676,
                 1680, 1684, 1688, 1692, 1696, 1704, 1708, 1712, 1716, 1720,
                 1724, 1728, 1732, 1736, 1740, 1744, 1748, 1752, 1756, 1760,
                 1764, 1768, 1772, 1776, 1780, 1784, 1788, 1792, 1796, 1804,
                 1808, 1812, 1816, 1820, 1824, 1828, 1832, 1836, 1840, 1844,
                 1848, 1852, 1856, 1860, 1864, 1868, 1872, 1876, 1880, 1884,
                 1888, 1892, 1896, 1904, 1908, 1912, 1916, 1920, 1924, 1928,
                 1932, 1936, 1940, 1944, 1948, 1952, 1956, 1960, 1964, 1968,
                 1972, 1976, 1980, 1984, 1988, 1992, 1996, 2000, 2004, 2008,
                 2012, 2016, 2020, 2024, 2028, 2032, 2036, 2040]
        for i in range(1600, 2041):
            if i in leaps:
                self.assertEqual(lib.Lgm_LeapYear(i), 1)
            else:
                self.assertEqual(lib.Lgm_LeapYear(i), 0)

    def test_Lgm_GetLeapSeconds(self):
        """Lgm_GetLeapSeconds should hvae known output for known input (regression)"""
        # even spaced JD from 1900 Jan8 to 2010 Dec22
        data = [ 2415028.        ,  2417160.89473684,  2419293.78947368,
        2421426.68421053,  2423559.57894737,  2425692.47368421,
        2427825.36842105,  2429958.26315789,  2432091.15789474,
        2434224.05263158,  2436356.94736842,  2438489.84210526,
        2440622.73684211,  2442755.63157895,  2444888.52631579,
        2447021.42105263,  2449154.31578947,  2451287.21052632,
        2453420.10526316,  2455553.        ]
        c = Lgm_CTrans.Lgm_CTrans()
        ans = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
               2.888061368417114, 8.091415894748657, 13.619879052638948,
               19.148342210528032, 24.676805368417114, 30.2052685263062,
               35.73373168422184, 41.262194842110915, 46.790658]
        for i, val in enumerate(data):
            self.assertAlmostEqual(lib.Lgm_GetLeapSeconds(val, c), ans[i])

    def test_Lgm_IsLeapSecondDay(self):
        """Lgm_IsLeapSecondDay should hvae known output for known input (regression)"""
        leapSdays = [19711231, 19720630, 19721231, 19731231, 19810630, 19871231,
                    19920630, 19951231]

        c = Lgm_CTrans.Lgm_CTrans()
        ans = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
               2.888061368417114, 8.091415894748657, 13.619879052638948,
               19.148342210528032, 24.676805368417114, 30.2052685263062,
               35.73373168422184, 41.262194842110915, 46.790658]
        secs = LgmDouble()
        for i, val in enumerate(leapSdays):
            out = lib.Lgm_IsLeapSecondDay(val, secs, c)
            secsv = secs.value
            self.assertAlmostEqual(ans[i], out)
            self.assertAlmostEqual(86400.0, secsv)

    def test_time_various(self):
        """Time conversions should work correctly"""
        utc = 2004.613427

    def test_dateToDateLong(self):
        """dateToDateLong should give known output"""
        t = datetime.datetime(2000, 12, 2)
        self.assertEqual(20001202, Lgm_DateAndTime.dateToDateLong(t))
        self.assertEqual([20001202, 20001202], Lgm_DateAndTime.dateToDateLong([t,t]))
        tmp = Lgm_DateAndTime.dateToDateLong(numpy.array([t,t]))
        for i, val in enumerate(numpy.array([20001202, 20001202])):
            self.assertEqual(val, tmp[i])

    def test_dateToFPHours(self):
        """dateToFPHours should return known output from known input"""
        t = datetime.datetime(2000, 12, 2)
        self.assertEqual(Lgm_DateAndTime.dateToFPHours(t), 0)
        t = datetime.datetime(2000, 12, 2, 12)
        self.assertEqual(Lgm_DateAndTime.dateToFPHours(t), 12.0)
        t = datetime.datetime(2000, 12, 2, 11, 40)
        self.assertAlmostEqual(Lgm_DateAndTime.dateToFPHours(t), 11.666666666666666)
        t = datetime.datetime(2000, 12, 2, 1, 1, 20)
        self.assertAlmostEqual(Lgm_DateAndTime.dateToFPHours(t), 1.0222222222222221)
        t = datetime.datetime(2000, 12, 2, 1, 1, 20, 34567)
        self.assertAlmostEqual(Lgm_DateAndTime.dateToFPHours(t), 1.0222318241666666)
        # and test arrays and lists
        t = datetime.datetime(2000, 12, 2, 12)
        self.assertEqual(Lgm_DateAndTime.dateToFPHours([t, t]), [12.0, 12.0])
        ans = Lgm_DateAndTime.dateToFPHours([t, t])
        self.assertTrue(isinstance(ans, list))
        ans = Lgm_DateAndTime.dateToFPHours(numpy.array([t, t]))
        for i, val in enumerate(ans):
            self.assertEqual(ans[i], 12.0)
        self.assertEqual(len(ans), 2)
        self.assertTrue(isinstance(ans, numpy.ndarray))

if __name__ == '__main__':
    unittest.main()