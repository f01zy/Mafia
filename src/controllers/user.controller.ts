import type { NextFunction, Request, Response } from "express";
import { MailService } from "../services/mail.service";
import { TokenService } from "../services/token.service";
import { UserService } from "../services/user.service";
import { prisma } from "../prisma";
import { validationResult } from "express-validator";
import { ApiError } from "../exceptions/api.exception";

const userService = new UserService()
const tokenService = new TokenService()
const mailService = new MailService()

export class UserController {
  public async register(req: Request, res: Response, next: NextFunction) {
    try {
      const errors = validationResult(req)
      if (!errors.isEmpty()) {
        return next(ApiError.BadRequest(errors.array()[0].msg, errors.array()))
      }

      const { username, email, password } = req.body
      const user = await userService.register(username, email, password)

      res.json(user)
    } catch (err) {
      next(err)
    }
  }

  public async login(req: Request, res: Response, next: NextFunction) {
    try {
      const errors = validationResult(req)
      if (!errors.isEmpty()) {
        return next(ApiError.BadRequest(errors.array()[0].msg, errors.array()))
      }

      const { email, password } = req.body
      const user = await userService.login(email, password)

      res.json(user)
    } catch (e) {
      next(e)
    }
  }

  public async logout(req: Request, res: Response, next: Function) {
    try {
      const { refreshToken } = req.body
      const token = await userService.logout(refreshToken)

      res.json(token)
    } catch (e) {
      next(e)
    }
  }

  public async activate(req: Request, res: Response, next: Function) {
    try {
      const activationCode = req.params.code

      await userService.activate(activationCode)
    } catch (e) {
      next(e)
    }
  }

  public async resendActivationCode(req: Request, res: Response, next: Function) {
    try {
      const { refreshToken } = req.body
      const user = await tokenService.getUserByRefreshToken(refreshToken)
      const activationCode = user.activationCode

      await mailService.sendActivationMail(user.email, activationCode)
    } catch (e) {
      next(e)
    }
  }

  public async refresh(req: Request, res: Response, next: Function) {
    try {
      const { refreshToken } = req.body
      const user = await userService.refresh(refreshToken)

      res.json(user)
    } catch (e) {
      next(e)
    }
  }

  public async getUserById(req: Request, res: Response, next: Function) {
    try {
      const { id } = req.params
      const userId = parseInt(id)
      const user = await prisma.user.findUnique({ where: { id: userId } })

      res.json(user)
    } catch (e) {
      next(e)
    }
  }
}
